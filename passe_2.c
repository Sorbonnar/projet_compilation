#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "common.h"
#include "arch.h"
#include "passe_2.h"
#include "miniccutils.h"

extern char * outfile;
extern int trace_level;
bool test = false;

void gen_code_passe_2(node_t root); // Fonction pour démarrer la génération de code
void gen_code_base(node_t node); // Fonction principale de génération de code
void gen_code_print(node_t node); // Fonction pour générer le code d'affichage
void gen_code_declaration(node_t node); // Fonction pour générer le code de déclaration
void gen_code_instruction(node_t node); // Fonction pour générer le code d'instruction
void gen_code_expression(node_t node); // Fonction pour générer le code d'expression

void gen_code_passe_2(node_t root) {
    printf_level(1, "\n################ Gencode Passe 2 ################\n");

    data_sec_inst_create();
    gen_code_declaration(root->opr[0]);
    for (int i = 0; i < get_global_strings_number(); i++)
        asciiz_inst_create(NULL, get_global_string(i));

    text_sec_inst_create();
    gen_code_base(root->opr[1]);
    stack_deallocation_inst_create(get_temporary_max_offset() + root->opr[1]->offset);
    ori_inst_create(2, 0, 10);
    syscall_inst_create();
}

void gen_code_base(node_t node) {
    if (node == NULL) return;

    switch (node->nature) {
        case NODE_BLOCK:
            printf_level(1, "gen_code_base \t\t: Exploring NODE_BLOCK\n");
            gen_code_declaration(node->opr[0]);
            gen_code_base(node->opr[1]);
            break;

        case NODE_FUNC:
            printf_level(1, "gen_code_base \t\t: Exploring NODE_FUNC\n");
            reset_temporary_max_offset();
            set_temporary_start_offset(node->offset);
            label_str_inst_create(node->opr[1]->ident);
            stack_allocation_inst_create();
            gen_code_base(node->opr[2]);
            break;

        case NODE_PRINT:
            printf_level(1, "gen_code_base \t\t: Exploring NODE_PRINT\n");
            gen_code_print(node);
            break;

        case NODE_IF: case NODE_WHILE: case NODE_DOWHILE:
        case NODE_FOR: case NODE_AFFECT:
            gen_code_instruction(node);
            break;

        case NODE_PLUS: case NODE_MINUS: case NODE_MUL: case NODE_DIV: case NODE_MOD: case NODE_AND: case NODE_BAND:
        case NODE_NOT: case NODE_LT: case NODE_GT: case NODE_LE: case NODE_GE: case NODE_EQ: case NODE_NE: case NODE_OR:
        case NODE_BOR: case NODE_BXOR: case NODE_SLL: case NODE_SRA: case NODE_SRL: case NODE_UMINUS: case NODE_BNOT:
        case NODE_INTVAL: case NODE_BOOLVAL: case NODE_STRINGVAL: case NODE_IDENT:
            gen_code_expression(node);
            break;

        case NODE_LIST:
            printf_level(1, "gen_code_base \t\t: Exploring NODE_LIST\n");
            for (int i = 0; i < node->nops; i++)
                gen_code_base(node->opr[i]);
            break;

        default:
            fprintf(stderr, "Error line %d: Unknown node type \'%s\' in generation\n", node->lineno, node_nature2string(node->nature));
            exit(1);
            break;
    }
}

void gen_code_print(node_t node) {
    if (node == NULL) return;

    for (int i = 0; i < node->nops; i++) {
        printf_level(1, "gen_code_print \t\t: ");
        printf_level(1, "Exploring NODE_%s\n", node_nature2string(node->opr[i]->nature));
        if (node->opr[i] != NULL) {
            if (node->opr[i]->nature == NODE_STRINGVAL) {
                lui_inst_create(4, 0x1001);
                ori_inst_create(4, 4, node->opr[i]->offset);
                ori_inst_create(2, 0, 4);
                syscall_inst_create();
            }
            else if (node->opr[i]->nature == NODE_IDENT) {
                if (node->opr[i]->decl_node != NULL) {
                    if (node->opr[i]->decl_node->global_decl) {
                        lui_inst_create(4, 0x1001);
                        lw_inst_create(4, node->opr[i]->decl_node->offset, 4);
                    }
                    else {
                        lw_inst_create(4, node->opr[i]->decl_node->offset, 29);
                    }
                }
                else {
                    fprintf(stderr, "Error line %d: Undeclared variable \'%s\'\n", node->lineno, node->ident);
                    exit(1);
                }
                ori_inst_create(2, 0, 1);
                syscall_inst_create();
            }
            else { // NODE_LIST
                gen_code_print(node->opr[i]);
            }
        }
    }

}

void gen_code_instruction(node_t node) {
    if (node == NULL) return;

    printf_level(1, "gen_code_instruction \t: ");
    printf_level(1, "Exploring NODE_%s\n", node_nature2string(node->nature));

    int32_t reg_loop;

    switch (node->nature) {
        case NODE_IF:
            gen_code_expression(node->opr[0]);

            if (node->nops > 2) {
                int32_t else_label = get_new_label();
                int32_t end_if_label = get_new_label();

                beq_inst_create(get_current_reg(), 0, else_label);
                gen_code_base(node->opr[1]);
                j_inst_create(end_if_label);
                label_inst_create(else_label);
                gen_code_base(node->opr[2]);
                label_inst_create(end_if_label);
            }
            else {
                int32_t end_if_label = get_new_label();

                beq_inst_create(get_current_reg(), 0, end_if_label);
                gen_code_base(node->opr[1]);
                label_inst_create(end_if_label);
            }
            break;

        case NODE_WHILE:
            int32_t while_loop_label = get_new_label();
            int32_t end_while_loop_label = get_new_label();

            label_inst_create(while_loop_label);
            reg_loop = get_current_reg();

            gen_code_expression(node->opr[0]);
            beq_inst_create(reg_loop, 0, end_while_loop_label);
            gen_code_base(node->opr[1]);
            j_inst_create(while_loop_label);
            label_inst_create(end_while_loop_label);
            break;

        case NODE_DOWHILE:
            int32_t do_while_loop_label = get_new_label();

            label_inst_create(do_while_loop_label);
            gen_code_base(node->opr[0]);
            gen_code_expression(node->opr[1]);
            bne_inst_create(get_current_reg(), 0, do_while_loop_label);
            break;

        case NODE_FOR:
            int32_t for_loop_label = get_new_label();
            int32_t end_for_loop_label = get_new_label();

            gen_code_instruction(node->opr[0]);
            label_inst_create(for_loop_label);

            reg_loop = get_current_reg();

            gen_code_expression(node->opr[1]);
            beq_inst_create(reg_loop, 0, end_for_loop_label);
            gen_code_base(node->opr[3]);
            gen_code_instruction(node->opr[2]);
            
            j_inst_create(for_loop_label);
            label_inst_create(end_for_loop_label);
            break;

        case NODE_AFFECT:
            int32_t reg = get_current_reg();
            gen_code_expression(node->opr[1]);

            if (node->opr[0]->decl_node != NULL) {
                int32_t numReg = node->opr[0]->decl_node->global_decl ? get_current_reg() : 29;

                if (!reg_available() && node->opr[0]->decl_node->global_decl) {
                    push_temporary(numReg);
                    lui_inst_create(numReg, 0x1001);
                    sw_inst_create(reg, node->opr[0]->decl_node->offset, numReg);
                    pop_temporary(numReg);
                }
                else if (node->opr[0]->decl_node->global_decl) {
                    allocate_reg();
                    lui_inst_create(get_current_reg(), 0x1001);
                    sw_inst_create(reg, node->opr[0]->decl_node->offset, get_current_reg());
                    release_reg();
                }
                else {
                    sw_inst_create(reg, node->opr[0]->decl_node->offset, numReg);
                }
            }
            else {
                fprintf(stderr, "Error line %d: Undeclared variable \'%s\'\n", node->lineno, node->opr[0]->ident);
                exit(1);
            }
            break;

        default:
            fprintf(stderr, "Error line %d: Unknown instruction type in generation\n", node->lineno);
            exit(1);
            break;
    }
}

void gen_code_expression(node_t node) {
    if (node == NULL) return;

    printf_level(1, "gen_code_expression \t: Exploring NODE_%s\n", node_nature2string(node->nature));

    int32_t reg = get_current_reg(), reg1, reg2;

    if (node->nature == NODE_IDENT) {

        if (node->decl_node != NULL) {
            if (node->decl_node->global_decl) {
                lui_inst_create(reg, 0x1001);
                lw_inst_create(reg, node->decl_node->offset, reg);
            }
            else {
                lw_inst_create(reg, node->decl_node->offset, 29);
            }
        }
        else {
            fprintf(stderr, "Error line %d: Undeclared variable \'%s\'\n", node->lineno, node->ident);
            exit(1);
        }
    }
    else if (node->nature == NODE_INTVAL || node->nature == NODE_BOOLVAL) {
        if (node->value == 0xFFFFFFFF) {
            addiu_inst_create(reg, 0, 0xFFFFFFFF);
        }
        else {
            int32_t up_value = node->value >> 16;
            int32_t low_value = node->value & 0xFFFF;

            if (up_value > 0) {
                lui_inst_create(reg, up_value);
                ori_inst_create(reg, reg, low_value);
            }
            else {
                ori_inst_create(reg, 0, low_value);
            }
        }
    }
    else {
        gen_code_expression(node->opr[0]);
        reg1 = get_current_reg();
        reg = reg1;

        if (!reg_available()) {
            push_temporary(reg1);
            reg1 = get_restore_reg();
        }
        else
            allocate_reg();

        reg2 = get_current_reg();
        if (node->nops > 1 && node->opr[1] != NULL)
            gen_code_expression(node->opr[1]);
        
        if (!reg_available() && get_restore_reg() == reg1)
            pop_temporary(reg1);

        switch (node->nature) {
            case NODE_PLUS:
                addu_inst_create(reg, reg1, reg2);
                break;
            case NODE_MINUS:
                subu_inst_create(reg, reg1, reg2);
                break;
            case NODE_MUL:
                mult_inst_create(reg1, reg2);
                mflo_inst_create(reg1); // LO register contains the product
                break;
            case NODE_DIV:
                div_inst_create(reg1, reg2);
                teq_inst_create(reg2, 0); // Trap the error if division by 0
                mflo_inst_create(reg1); // LO register contains the quotient
                break;
            case NODE_MOD:
                div_inst_create(reg1, reg2);
                teq_inst_create(reg2, 0); // Same as div
                mfhi_inst_create(reg1); // HI register contains the remainder
                break;
            case NODE_BAND:
                and_inst_create(reg, reg1, reg2);
                break;
            case NODE_BOR:
                or_inst_create(reg, reg1, reg2);
                break;
            case NODE_BXOR:
                xor_inst_create(reg, reg1, reg2);
                break;
            case NODE_AND:
                and_inst_create(reg, reg1, reg2);
                break;
            case NODE_OR:
                or_inst_create(reg, reg1, reg2);
                break;
            case NODE_EQ:
                xor_inst_create(reg1, reg1, reg2); // Bits difference between the two regs
                sltiu_inst_create(reg1, reg1, 1); // If the result is 0, the two regs are equals and reg1 is true
                break;
            case NODE_NE:
                xor_inst_create(reg1, reg1, reg2); // Same as NODE_EQ
                sltu_inst_create(reg1, 0, reg1); // If the result is greater than 0, the two regs are different and reg1 is true
                break;
            case NODE_LT:
                slt_inst_create(reg1, reg1, reg2);
                break;
            case NODE_GT:
                slt_inst_create(reg1, reg2, reg1);
                break;
            case NODE_LE:
                slt_inst_create(reg1, reg2, reg1);
                xori_inst_create(reg1, reg1, 1);
                break;
            case NODE_GE:
                slt_inst_create(reg1, reg1, reg2);
                xori_inst_create(reg1, reg1, 1);
                break;
            case NODE_SLL:
                sllv_inst_create(reg, reg1, reg2);
                break;
            case NODE_SRL:
                srlv_inst_create(reg, reg1, reg2);
                break;
            case NODE_SRA:
                srav_inst_create(reg, reg1, reg2);
                break;
            case NODE_NOT:
                nor_inst_create(reg, 0, reg1);
                break;
            case NODE_UMINUS:
                subu_inst_create(reg, 0, reg1);
                break;
            case NODE_BNOT:
                xori_inst_create(reg, reg1, 1);
                break;
            default:
                fprintf(stderr, "Error line %d: Unknown expression type \'%s\' in generation\n", node->lineno, node_nature2string(node->nature));
                exit(1);
                break;
        }

        if (reg_available() || get_restore_reg() != reg1)
            release_reg();
    }
}

void gen_code_declaration(node_t node) {
    if (node == NULL) return;

    printf_level(1, "gen_code_declaration \t: ");
    printf_level(1, "Exploring NODE_%s\n", node_nature2string(node->nature));

    if (node->nature == NODE_LIST) {
        gen_code_declaration(node->opr[0]);
        gen_code_declaration(node->opr[1]);
    }
    else if (node->nature == NODE_DECLS) {
        gen_code_declaration(node->opr[1]);
    }
    else if (node->nature == NODE_DECL) {
        printf_level(2, "In GenCode \t\t: Declaring %s \'%s\'\n", node->opr[0]->type == TYPE_INT ? "integer" : "boolean", node->opr[0]->ident);
        if (node->opr[0]->global_decl) {
            word_inst_create(node->opr[0]->ident, node->opr[1]->value);
        }
        else if (node->opr[1] != NULL) {
            gen_code_expression(node->opr[1]);
            sw_inst_create(get_current_reg(), node->opr[0]->offset, 29);
        }
    }
    else {
        fprintf(stderr, "Error line %d: Unknown declaration type \'%s\' in generation\n", node->lineno, node_nature2string(node->nature));
        exit(1);
    }
}
