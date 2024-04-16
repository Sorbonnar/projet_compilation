#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "common.h"
#include "arch.h"
#include "passe_2.h"
#include "miniccutils.h"

extern char * outfile;
bool test = false;

void parcours_gen(node_t node);
void create_print(node_t node);
void create_declaration(node_t node);
void create_instruction(node_t node);
void create_expression(node_t node);

void gen_code_passe_2(node_t root) {

    data_sec_inst_create();
    create_declaration(root->opr[0]);
    for (int i = 0; i < get_global_strings_number(); i++) {
        asciiz_inst_create(NULL, get_global_string(i));
    }

    text_sec_inst_create();
    parcours_gen(root->opr[1]);
    stack_deallocation_inst_create(get_temporary_max_offset() + root->opr[1]->offset);
    ori_inst_create(2, 0, 10);
    syscall_inst_create();
}

void parcours_gen(node_t node) {
    if (node == NULL) return;

    switch (node->nature) {
        case NODE_BLOCK:
            create_declaration(node->opr[0]);
            parcours_gen(node->opr[1]);
            break;

        case NODE_FUNC:
            reset_temporary_max_offset();
            set_temporary_start_offset(node->offset);
            label_str_inst_create(node->opr[1]->ident);
            stack_allocation_inst_create();
            parcours_gen(node->opr[2]);
            break;

        case NODE_PRINT:
            create_print(node);
            break;

        case NODE_IF: case NODE_WHILE: case NODE_DOWHILE:
        case NODE_FOR: case NODE_AFFECT:
            create_instruction(node);
            break;

        case NODE_PLUS: case NODE_MINUS: case NODE_MUL: case NODE_DIV: case NODE_MOD: case NODE_AND: case NODE_BAND:
        case NODE_NOT: case NODE_LT: case NODE_GT: case NODE_LE: case NODE_GE: case NODE_EQ: case NODE_NE: case NODE_OR:
        case NODE_BOR: case NODE_BXOR: case NODE_SLL: case NODE_SRA: case NODE_SRL: case NODE_UMINUS: case NODE_BNOT:
        case NODE_INTVAL: case NODE_BOOLVAL: case NODE_STRINGVAL: case NODE_IDENT:
            create_expression(node);
            break;

        case NODE_LIST:
            for (int i = 0; i < node->nops; i++)
                parcours_gen(node->opr[i]);
            break;

        default:
            fprintf(stderr, "Error line %d: Unknown node type in generation\n", node->lineno);
            exit(1);
            break;
    }
}

void create_print(node_t node) {
    if (node == NULL) return;

    for (int i = 0; i < node->nops; i++) {
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
                        lw_inst_create(4, node->opr[i]->decl_node->offset, get_current_reg());
                    }
                    else {
                        lw_inst_create(4, node->opr[i]->decl_node->offset, 29);
                    }
                }
                else {
                    fprintf(stderr, "Error line %d: Undeclared variable %s.\n", node->lineno, node->ident);
                    exit(1);
                }
                ori_inst_create(2, 0, 1);
                syscall_inst_create();
            }
            else {
                create_print(node->opr[i]);
            }
        }
    }

}

void create_instruction(node_t node) {
    if (node == NULL) return;

    switch (node->nature) {
        case NODE_IF:
            create_expression(node->opr[0]);

            if (node->nops > 2) {
                int32_t else_label = get_new_label();
                int32_t end_if_label = get_new_label();

                beq_inst_create(get_current_reg(), 0, else_label);
                parcours_gen(node->opr[1]);
                j_inst_create(end_if_label);
                label_inst_create(else_label);
                parcours_gen(node->opr[2]);
                label_inst_create(end_if_label);
            }
            else {
                int32_t end_if_label = get_new_label();

                beq_inst_create(get_current_reg(), 0, end_if_label);
                parcours_gen(node->opr[1]);
                label_inst_create(end_if_label);
            }
            break;

        case NODE_WHILE:
            int32_t while_loop_label = get_new_label();
            int32_t end_while_loop_label = get_new_label();

            label_inst_create(while_loop_label);
            create_expression(node->opr[0]);
            beq_inst_create(get_current_reg(), 0, end_while_loop_label);
            parcours_gen(node->opr[1]);
            j_inst_create(while_loop_label);
            label_inst_create(end_while_loop_label);
            break;

        case NODE_DOWHILE:
            int32_t do_while_loop_label = get_new_label();

            label_inst_create(do_while_loop_label);
            parcours_gen(node->opr[0]);
            create_expression(node->opr[1]);
            bne_inst_create(get_current_reg(), 0, do_while_loop_label);
            break;

        case NODE_FOR:
            int32_t for_loop_label = get_new_label();
            int32_t end_for_loop_label = get_new_label();

            create_instruction(node->opr[0]);
            label_inst_create(for_loop_label);

            create_expression(node->opr[1]);
            beq_inst_create(get_current_reg(), 0, end_for_loop_label);
            parcours_gen(node->opr[3]);
            create_instruction(node->opr[2]);
            
            j_inst_create(for_loop_label);
            label_inst_create(end_for_loop_label);
            break;

        case NODE_AFFECT:
            int32_t reg = get_current_reg();
            create_expression(node->opr[1]);

            if (node->opr[0]->decl_node != NULL) {
                int32_t numReg = node->opr[0]->decl_node->global_decl ? get_current_reg() : 29;

                if (!reg_available() && node->opr[0]->decl_node->global_decl) {
                    push_temporary(numReg);
                    lui_inst_create(numReg, 0x1001);
                }

                sw_inst_create(reg, node->opr[0]->decl_node->offset, numReg);

                if (!reg_available() && node->opr[0]->decl_node->global_decl) {
                    pop_temporary(numReg);
                }
            }
            else {
                fprintf(stderr, "Error line %d: Undeclared variable %s.\n", node->lineno, node->opr[0]->ident);
                exit(1);
            }
            break;


        default:
            fprintf(stderr, "Error line %d: Unknown instruction type in generation.\n", node->lineno);
            exit(1);
            break;
    }
}

void create_expression(node_t node) {
    if (node == NULL) return;

    int32_t reg = get_current_reg(), reg1, reg2;

    if (node->nature == NODE_IDENT) {
        if (!reg_available()) {
            push_temporary(reg);
        }

        if (node->decl_node != NULL) {
            if (node->decl_node->global_decl) {
                lui_inst_create(reg, 0x1001);
                lw_inst_create(reg, node->decl_node->offset, reg);
            } else {
                lw_inst_create(reg, node->decl_node->offset, 29);
            }
        }
        else {
            fprintf(stderr, "Error line %d: Undeclared variable %s.\n", node->lineno, node->ident);
            exit(1);
        }

        if (!reg_available()) {
            pop_temporary(reg);
        }
    }
    else if (node->nature == NODE_INTVAL || node->nature == NODE_BOOLVAL) {
        ori_inst_create(reg, 0, node->value);
    }
    else {
        create_expression(node->opr[0]);
        reg1 = get_current_reg();
        reg = reg1;

        if (!reg_available()) {
            push_temporary(reg1);
            reg1 = get_restore_reg();
        }
        else {
            allocate_reg();
        }

        reg2 = get_current_reg();
        create_expression(node->opr[1]);

        if (!reg_available() && get_restore_reg() == reg1) {
            pop_temporary(reg1);
        }

        switch (node->nature) {
            case NODE_PLUS:
                addu_inst_create(reg, reg1, reg2);
                break;
            case NODE_MINUS:
                subu_inst_create(reg, reg1, reg2);
                break;
            case NODE_MUL:
                mult_inst_create(reg1, reg2);
                mflo_inst_create(reg1);
                break;
            case NODE_DIV:
                div_inst_create(reg1, reg2);
                teq_inst_create(reg2, 0);
                mflo_inst_create(reg1);
                break;
            case NODE_MOD:
                div_inst_create(reg1, reg2);
                teq_inst_create(reg2, 0);
                mfhi_inst_create(reg1);
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
                xor_inst_create(reg1, reg1, reg2);
                sltiu_inst_create(reg1, reg1, 1);
                break;
            case NODE_NE:
                xor_inst_create(reg1, reg1, reg2);
                sltu_inst_create(reg1, 0, reg1);
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
        }

        if (reg_available()) {
            release_reg();
        }
    }
}

void create_declaration(node_t node) {
    if (node == NULL) return;

    if (node->nature == NODE_LIST) {
        create_declaration(node->opr[0]);
        create_declaration(node->opr[1]);
    }
    else if (node->nature == NODE_DECLS) {
        create_declaration(node->opr[1]);
    }
    else if (node->nature == NODE_DECL) {
        if (node->opr[0]->global_decl) {
            word_inst_create(node->opr[0]->ident, node->opr[1]->value);
        }
        else if (node->opr[1] != NULL) {
            if (!reg_available()) {
                push_temporary(get_current_reg());
            }

            create_expression(node->opr[1]);
            sw_inst_create(get_current_reg(), node->opr[0]->offset, 29);

            if (!reg_available()) {
                pop_temporary(get_current_reg());
            }
        }
    }
}
