#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "common.h"
#include "arch.h"
#include "passe_1.h"
#include "miniccutils.h"

extern int trace_level;

void parcours_base(node_t node);
void process_expression(node_t node);
void process_instruction(node_t node);
void process_declaration(bool global, node_t node, node_type type);
 
void analyse_passe_1(node_t root) {
    push_global_context();
    process_declaration(1, root->opr[0], TYPE_NONE);
    if (root->opr[1] != NULL || root->opr[1]->nature != NODE_FUNC) {
        parcours_base(root->opr[1]);
    }
    else {
        fprintf(stderr, "Error line %d: No main function.\n", root->lineno);
        exit(1);
    }
}
  
void parcours_base(node_t node) {
    if (node == NULL) return;

    switch (node->nature) {
        case NODE_BLOCK:
            push_context();
            process_declaration(0, node->opr[0], TYPE_NONE);
            parcours_base(node->opr[1]);
            pop_context();
            break;

        case NODE_FUNC:
            reset_env_current_offset();
            push_context();
            if (node->nops < 3) {
                fprintf(stderr, "Error line %d: Function node does not have enough son.\n", node->lineno);
                exit(1);
            }
            else if (node->opr[0]->type != TYPE_VOID) {
                fprintf(stderr, "Error line %d: Function node does not have the correct type.\n", node->lineno);
                exit(1);
            }
            else if (node->opr[1]->nature != NODE_IDENT || strcmp(node->opr[1]->ident, "main") != 0) {
                fprintf(stderr, "Error line %d: No main function.\n", node->lineno);
                exit(1);
            }
            else if (node->opr[2]->nature != NODE_BLOCK) {
                fprintf(stderr, "Error line %d: Function node does not have a block as its third operand.\n", node->lineno);
                exit(1);
            }
            parcours_base(node->opr[2]);
            node->offset = get_env_current_offset();
            pop_context();
            break;

        case NODE_IF: case NODE_WHILE: case NODE_DOWHILE:
        case NODE_FOR: case NODE_PRINT: case NODE_AFFECT:
            process_instruction(node);
            break;

        case NODE_PLUS: case NODE_MINUS: case NODE_MUL: case NODE_DIV: case NODE_MOD: case NODE_AND: case NODE_BAND:
        case NODE_NOT: case NODE_LT: case NODE_GT: case NODE_LE: case NODE_GE: case NODE_EQ: case NODE_NE: case NODE_OR:
        case NODE_BOR: case NODE_BXOR: case NODE_SLL: case NODE_SRA: case NODE_SRL: case NODE_UMINUS: case NODE_BNOT:
        case NODE_INTVAL: case NODE_BOOLVAL: case NODE_STRINGVAL: case NODE_IDENT:
            process_expression(node);
            break;

        case NODE_LIST:
            for (int i = 0; i < node->nops; i++) {
                parcours_base(node->opr[i]);
            }
            break;

        default:
            fprintf(stderr, "Error line %d: Unknown node type\n", node->lineno);
            exit(1);
            break;
    }
}

void process_instruction(node_t node) {
    if (node == NULL) return;

    switch (node->nature) {
        case NODE_IF:
            process_expression(node->opr[0]);
            parcours_base(node->opr[1]);
            if (node->nops > 2) {
                parcours_base(node->opr[2]);
            }
            break;

        case NODE_WHILE:
            process_expression(node->opr[0]);
            parcours_base(node->opr[1]);
            break;

        case NODE_DOWHILE:
            process_expression(node->opr[1]);
            parcours_base(node->opr[0]);
            break;

        case NODE_FOR:
            process_instruction(node->opr[0]);
            process_expression(node->opr[1]);
            process_instruction(node->opr[2]);
            parcours_base(node->opr[3]);
            break;

        case NODE_PRINT:
            for (int i = 0; i < node->nops; ++i) {
                if (node->opr[i] != NULL) parcours_base(node->opr[i]);
            }
            break;

        case NODE_AFFECT:
            if (node->opr[0]->nature != NODE_IDENT) {
                fprintf(stderr, "Error line %d: Affectation to a non-identifier.\n", node->lineno);
                exit(1);
            }
            process_expression(node->opr[0]);
            process_expression(node->opr[1]);

            if (node->opr[0]->type != node->opr[1]->type) {
                fprintf(stderr, "Error line %d: Incorrect type for affectation.\n", node->lineno);
                exit(1);
            }

            if (node->opr[0]->type != TYPE_INT && node->opr[0]->type != TYPE_BOOL) {
                fprintf(stderr, "Error line %d: Invalid type for affectation.\n", node->lineno);
                exit(1);
            }
            break;

        default:
            fprintf(stderr, "Error line %d: Unknown instruction type.\n", node->lineno);
            exit(1);
    }
}

void process_expression(node_t node) {
    if (node == NULL) return;

    switch (node->nature) {
        case NODE_PLUS: case NODE_MINUS: case NODE_MUL: case NODE_DIV:
        case NODE_MOD: case NODE_BAND: case NODE_BOR: case NODE_BXOR:
        case NODE_SLL: case NODE_SRL: case NODE_SRA:
            process_expression(node->opr[0]);
            process_expression(node->opr[1]);

            if (node->opr[0]->type != TYPE_INT || node->opr[1]->type != TYPE_INT) {
                fprintf(stderr, "Error line %d: Incorrect type for operands.\n", node->lineno);
                exit(1);
            }

            node->type = TYPE_INT;
            break;

        case NODE_EQ: case NODE_NE: case NODE_LT:
        case NODE_GT: case NODE_LE: case NODE_GE:
            process_expression(node->opr[0]);
            process_expression(node->opr[1]);

            if (node->opr[0]->type != TYPE_INT || node->opr[1]->type != TYPE_INT) {
                fprintf(stderr, "Error line %d: Incorrect type for comparison operands.\n", node->lineno);
                exit(1);
            }

            node->type = TYPE_BOOL;
            break;

        case NODE_AND: case NODE_OR:
            process_expression(node->opr[0]);
            process_expression(node->opr[1]);

            if (node->opr[0]->type != TYPE_BOOL || node->opr[1]->type != TYPE_BOOL) {
                fprintf(stderr, "Error line %d: Incorrect type for logical operation operands.\n", node->lineno);
                exit(1);
            }

            node->type = TYPE_BOOL;
            break;

        case NODE_UMINUS: case NODE_NOT:
            process_expression(node->opr[0]);

            if (node->opr[0]->type != TYPE_INT) {
                fprintf(stderr, "Error line %d: Incorrect type for NOT operation operand.\n", node->lineno);
                exit(1);
            }

            node->type = TYPE_INT;
            break;

        case NODE_BNOT:
            process_expression(node->opr[0]);

            if (node->opr[0]->type != TYPE_BOOL) {
                fprintf(stderr, "Error line %d: Incorrect type for unary operation operand.\n", node->lineno);
                exit(1);
            }

            node->type = TYPE_BOOL;
            break;

        case NODE_INTVAL:
            node->type = TYPE_INT;
            break;

        case NODE_BOOLVAL:
            node->type = TYPE_BOOL;
            break;

        case NODE_STRINGVAL:
            int off = -1;
            if (node->str != NULL) off = add_string(node->str);
            if (off >= 0)
                node->offset = off;
            break;

        case NODE_IDENT:
            void *decl_node = get_decl_node(node->ident);
            if (decl_node == NULL) {
                fprintf(stderr, "Error line %d: Undeclared variable %s.\n", node->lineno, node->ident);
                exit(1);
            }
            node->decl_node = decl_node;
            node->type = ((node_t)decl_node)->type;
            break;

        default:
            fprintf(stderr, "Error line %d: Unknown expression type.\n", node->lineno);
            exit(1);
            break;
    }
}

void process_declaration(bool global, node_t node, node_type type) {
    if (node == NULL) return;

    if (node->nature == NODE_LIST) {
        process_declaration(global, node->opr[0], type);
        process_declaration(global, node->opr[1], type);
    }
    else if (node->nature == NODE_DECLS) {
        process_declaration(global, node->opr[1], node->opr[0]->type);
    }
    else if (node->nature == NODE_DECL) {
        int off = env_add_element(node->opr[0]->ident, node->opr[0]);
        node->opr[0]->global_decl = global;
        node->opr[0]->type = type;
        
        if (node->opr[1] != NULL) {
            process_expression(node->opr[1]);

            if (node->opr[1]->type != type) {
                fprintf(stderr, "Error line %d: Incorrect type for variable %s.\n", node->lineno, node->opr[0]->ident);
                exit(1);
            }
        }
        else if (global) {
            node->opr[1] = malloc(sizeof(node_t));
            node->opr[1]->type = type;
            if (type == TYPE_INT) {
                node->opr[1]->nature = NODE_INTVAL;
                node->opr[1]->value = 10;
            }
            else if (type == TYPE_BOOL) {
                node->opr[1]->nature = NODE_BOOLVAL;
                node->opr[1]->value = false;
            }
        }

        if (off >= 0)
            node->opr[0]->offset = off;
        else {
            fprintf(stderr, "Error line %d: Variable %s already declared in this context\n", node->lineno, node->opr[0]->ident);
            exit(1);
        }
    }
}
