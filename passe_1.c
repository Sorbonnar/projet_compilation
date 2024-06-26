#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "common.h"
#include "arch.h"
#include "passe_1.h"
#include "miniccutils.h"

extern int trace_level;

const char* get_symbol(node_nature node); // Fonction pour obtenir le symbole d'un noeud, utilisée seulement pour l'affichage
void analyse_passe_1(node_t root); // Fonction pour démarrer le parcours
void analyse_base(node_t node); // Fonction principale pour le parcours de l'arbre
void analyse_instruction(node_t node); // Fonction qui analyse les instructions
void analyse_expression(node_t node); // Fonction qui analyse les expressions
void analyse_declaration(bool global, node_t node, node_type type); // Fonction qui analyse les déclarations

const char* get_symbol(node_nature node) {
    switch (node) {
        case NODE_PLUS: return "+";
        case NODE_MINUS: return "-";
        case NODE_MUL: return "*";
        case NODE_DIV: return "/";
        case NODE_MOD: return "%";
        case NODE_BAND: return "&";
        case NODE_BOR: return "|";
        case NODE_BXOR: return "^";
        case NODE_SLL: return "<<";
        case NODE_SRL: return ">>";
        case NODE_SRA: return ">>>";
        case NODE_EQ: return "==";
        case NODE_NE: return "!=";
        case NODE_LT: return "<";
        case NODE_GT: return ">";
        case NODE_LE: return "<=";
        case NODE_GE: return ">=";
        case NODE_AND: return "&&";
        case NODE_OR: return "||";
        case NODE_UMINUS: return "-";
        case NODE_NOT: return "!";
        case NODE_BNOT: return "~";
        default: return "?";
    }
}
 
void analyse_passe_1(node_t root) {
    printf_level(3, "\n################ Analyse Passe 1 ################\n");

    push_global_context();
    analyse_declaration(1, root->opr[0], TYPE_NONE);
    if (root->opr[1] != NULL && root->opr[1]->nature == NODE_FUNC) {
        analyse_base(root->opr[1]);
    }
    else {
        fprintf(stderr, "Error line %d: no main function\n", root->lineno);
        exit(1);
    }
    pop_context();
}
  
void analyse_base(node_t node) {
    if (node == NULL) return;

    switch (node->nature) {
        case NODE_BLOCK:
            printf_level(3, "analyse_base \t\t: exploring node NODE_BLOCK\n");
            push_context();
            analyse_declaration(0, node->opr[0], TYPE_NONE);
            analyse_base(node->opr[1]);
            pop_context();
            break;

        case NODE_FUNC:
            printf_level(3, "analyse_base \t\t: exploring node NODE_FUNC\n");
            reset_env_current_offset();
            push_context();
            if (node->nops < 3) {
                fprintf(stderr, "Error line %d: function node is incomplete\n", node->opr[0]->lineno);
                exit(1);
            }
            else if (node->opr[0]->type != TYPE_VOID) {
                fprintf(stderr, "Error line %d: function node does not have the correct type \'void\'\n", node->opr[0]->lineno);
                exit(1);
            }
            else if (node->opr[1]->nature != NODE_IDENT || strcmp(node->opr[1]->ident, "main") != 0) {
                fprintf(stderr, "Error line %d: no main function\n", node->opr[0]->lineno);
                exit(1);
            }
            else if (node->opr[2]->nature != NODE_BLOCK) {
                fprintf(stderr, "Error line %d: function node does not have a block\n", node->opr[0]->lineno);
                exit(1);
            }
            analyse_base(node->opr[2]);
            node->offset = get_env_current_offset();
            pop_context();
            break;

        case NODE_IF: case NODE_WHILE: case NODE_DOWHILE:
        case NODE_FOR: case NODE_PRINT: case NODE_AFFECT:
            analyse_instruction(node);
            break;

        case NODE_PLUS: case NODE_MINUS: case NODE_MUL: case NODE_DIV: case NODE_MOD: case NODE_AND: case NODE_BAND:
        case NODE_NOT: case NODE_LT: case NODE_GT: case NODE_LE: case NODE_GE: case NODE_EQ: case NODE_NE: case NODE_OR:
        case NODE_BOR: case NODE_BXOR: case NODE_SLL: case NODE_SRA: case NODE_SRL: case NODE_UMINUS: case NODE_BNOT:
        case NODE_INTVAL: case NODE_BOOLVAL: case NODE_STRINGVAL: case NODE_IDENT:
            analyse_expression(node);
            break;

        case NODE_LIST:
            printf_level(3, "analyse_base \t\t: exploring node NODE_LIST\n");
            for (int i = 0; i < node->nops; i++) analyse_base(node->opr[i]);
            break;

        default:
            fprintf(stderr, "Error line %d: unknown node type \'%s\'\n", node->lineno, node_nature2string(node->nature));
            exit(1);
            break;
    }
}

void analyse_instruction(node_t node) {
    if (node == NULL) return;

    printf_level(3, "analyse_instruction \t: ");
    printf_level(3, "exploring node NODE_%s\n", node_nature2string(node->nature));

    switch (node->nature) {
        case NODE_IF:
            analyse_expression(node->opr[0]);
            analyse_base(node->opr[1]);
            if (node->nops > 2) analyse_base(node->opr[2]);

            if (node->opr[0]->type != TYPE_BOOL) {
                fprintf(stderr, "Error line %d: incorrect type for \'if\' condition, boolean expected\n", node->opr[0]->lineno);
                exit(1);
            }
            break;

        case NODE_WHILE:
            analyse_expression(node->opr[0]);
            analyse_base(node->opr[1]);

            if (node->opr[0]->type != TYPE_BOOL) {
                fprintf(stderr, "Error line %d: incorrect type for \'while\' loop condition, boolean expected\n", node->opr[0]->lineno);
                exit(1);
            }
            break;

        case NODE_DOWHILE:
            analyse_expression(node->opr[1]);
            analyse_base(node->opr[0]);

            if (node->opr[1]->type != TYPE_BOOL) {
                fprintf(stderr, "Error line %d: incorrect type for \'do-while\' loop condition, boolean expected\n", node->opr[1]->lineno);
                exit(1);
            }
            break;

        case NODE_FOR:
            analyse_instruction(node->opr[0]);
            analyse_expression(node->opr[1]);
            analyse_instruction(node->opr[2]);
            analyse_base(node->opr[3]);

            if (node->opr[1]->type != TYPE_BOOL) {
                fprintf(stderr, "Error line %d: incorrect type for \'for\' loop condition, boolean expected\n", node->opr[1]->lineno);
                exit(1);
            }
            break;

        case NODE_PRINT:
            for (int i = 0; i < node->nops; ++i) if (node->opr[i] != NULL) analyse_base(node->opr[i]);
            break;

        case NODE_AFFECT:
            if (node->opr[0]->nature != NODE_IDENT) {
                fprintf(stderr, "Error line %d: affectation to a non-identifier\n", node->lineno);
                exit(1);
            }
            analyse_expression(node->opr[0]);
            analyse_expression(node->opr[1]);

            if (node->opr[0]->type != node->opr[1]->type) {
                fprintf(stderr, "Error line %d: incorrect type for affectation to variable \'%s\'\n", node->lineno, node->opr[0]->ident);
                exit(1);
            }

            if (node->opr[0]->type != TYPE_INT && node->opr[0]->type != TYPE_BOOL) {
                fprintf(stderr, "Error line %d: \'%s\' is of invalid type\n", node->lineno, node->opr[0]->ident);
                exit(1);
            }
            break;

        default:
            fprintf(stderr, "Error line %d: unknown instruction type \'%s\'\n", node->lineno, node_nature2string(node->nature));
            exit(1);
    }
}

void analyse_expression(node_t node) {
    if (node == NULL) return;

    printf_level(3, "analyse_expression \t: ");
    printf_level(3, "exploring node NODE_%s\n", node_nature2string(node->nature));

    switch (node->nature) {
        case NODE_PLUS: case NODE_MINUS: case NODE_MUL: case NODE_DIV:
        case NODE_MOD: case NODE_BAND: case NODE_BOR: case NODE_BXOR:
        case NODE_SLL: case NODE_SRL: case NODE_SRA:
            analyse_expression(node->opr[0]);
            analyse_expression(node->opr[1]);

            if (node->opr[0]->type != TYPE_INT) {
                fprintf(stderr, "Error line %d: \'%s\' is of incorrect type for arithmetic operation \'%s\', integer expected\n", node->lineno, node->opr[0]->ident, get_symbol(node->nature));
                exit(1);
            }
            else if (node->opr[1]->type != TYPE_INT) {
                fprintf(stderr, "Error line %d: \'%s\' is of incorrect type for arithmetic operation \'%s\', integer expected\n", node->lineno, node->opr[1]->ident, get_symbol(node->nature));
                exit(1);
            }

            node->type = TYPE_INT;
            break;

        case NODE_EQ: case NODE_NE: case NODE_LT:
        case NODE_GT: case NODE_LE: case NODE_GE:
            analyse_expression(node->opr[0]);
            analyse_expression(node->opr[1]);

            if (node->opr[0]->type != TYPE_INT) {
                fprintf(stderr, "Error line %d: \'%s\' is of incorrect type for comparison \'%s\', integer expected\n", node->lineno, node->opr[0]->ident, get_symbol(node->nature));
                exit(1);
            }
            else if (node->opr[1]->type != TYPE_INT) {
                fprintf(stderr, "Error line %d: \'%s\' is of incorrect type for comparison \'%s\', integer expected\n", node->lineno, node->opr[1]->ident, get_symbol(node->nature));
                exit(1);
            }

            node->type = TYPE_BOOL;
            break;

        case NODE_AND: case NODE_OR:
            analyse_expression(node->opr[0]);
            analyse_expression(node->opr[1]);

            if (node->opr[0]->type != TYPE_BOOL) {
                fprintf(stderr, "Error line %d: \'%s\' is of incorrect type for logical operation \'%s\', boolean expected\n", node->lineno, node->opr[0]->ident, get_symbol(node->nature));
                exit(1);
            }
            else if (node->opr[1]->type != TYPE_BOOL) {
                fprintf(stderr, "Error line %d: \'%s\' is of incorrect type for logical operation \'%s\', boolean expected\n", node->lineno, node->opr[1]->ident, get_symbol(node->nature));
                exit(1);
            }

            node->type = TYPE_BOOL;
            break;

        case NODE_UMINUS: case NODE_BNOT:
            analyse_expression(node->opr[0]);

            if (node->opr[0]->type != TYPE_INT) {
                fprintf(stderr, "Error line %d: \'%s\' is of incorrect type for unary operation \'%s\', integer expected\n", node->lineno, node->opr[0]->ident, get_symbol(node->nature));
                exit(1);
            }

            node->type = TYPE_INT;
            break;

        case NODE_NOT:
            analyse_expression(node->opr[0]);

            if (node->opr[0]->type != TYPE_BOOL) {
                fprintf(stderr, "Error line %d: \'%s\' is of incorrect type for unary operation \'%s\', boolean expected\n", node->lineno, node->opr[0]->ident, get_symbol(node->nature));
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
            if (off >= 0) node->offset = off;
            break;

        case NODE_IDENT:
            void *decl_node = get_decl_node(node->ident);
            if (decl_node == NULL) {
                fprintf(stderr, "Error line %d: undeclared variable \'%s\'\n", node->lineno, node->ident);
                exit(1);
            }
            node->decl_node = decl_node;
            node->type = ((node_t)decl_node)->type;
            break;

        default:
            fprintf(stderr, "Error line %d: unknown expression type \'%s\'\n", node->lineno, node_nature2string(node->nature));
            exit(1);
            break;
    }
}

void analyse_declaration(bool global, node_t node, node_type type) {
    if (node == NULL) return;

    int off = -1;

    printf_level(3, "analyse_declaration \t: ");
    printf_level(3, "exploring node NODE_%s\n", node_nature2string(node->nature));

    if (node->nature == NODE_LIST) {
        analyse_declaration(global, node->opr[0], type);
        analyse_declaration(global, node->opr[1], type);
    }
    else if (node->nature == NODE_DECLS) {
        analyse_declaration(global, node->opr[1], node->opr[0]->type);
    }
    else if (node->nature == NODE_DECL) {
        printf_level(4, "In analyse \t\t: declaring %s \'%s\'\n", type == TYPE_INT ? "integer" : "boolean", node->opr[0]->ident);

        if (type == TYPE_VOID) {
            fprintf(stderr, "Error line %d: variable of type void\n", node->lineno);
            exit(1);
        }

        if (node->opr[0] != NULL) {
            off = env_add_element(node->opr[0]->ident, node->opr[0]);
            node->opr[0]->global_decl = global;
            node->opr[0]->type = type;
        }
        if (node->opr[1] != NULL) {
            if (global && node->opr[1]->nature != NODE_INTVAL && node->opr[1]->nature != NODE_BOOLVAL) {
                fprintf(stderr, "Error line %d: global variables must be initialized with a constant value\n", node->lineno);
                exit(1);
            }

            analyse_expression(node->opr[1]);

            if (node->opr[1]->type != type) {
                fprintf(stderr, "Error line %d: \'%s\' is of incorrect type for declaration, %s expected\n", node->lineno, node->opr[0]->ident, type == TYPE_INT ? "integer" : "boolean");
                exit(1);
            }
        }
        else if (global) {
            node->opr[1] = make_node(NONE, 0);
            node->opr[1]->type = type;
            if (type == TYPE_INT) {
                node->opr[1]->nature = NODE_INTVAL;
                node->opr[1]->value = 0;
            }
            else if (type == TYPE_BOOL) {
                node->opr[1]->nature = NODE_BOOLVAL;
                node->opr[1]->value = false;
            }
            else if (type == TYPE_VOID) {
                fprintf(stderr, "Error line %d: type void can't be attributed to a variable\n", node->lineno);
                exit(1);
            }
            else {
                fprintf(stderr, "Error line %d: unknown declaration type \'%s\'\n", node->lineno, node_nature2string(node->nature));
                exit(1);
            }
        }

        if (off >= 0) {
            node->opr[0]->offset = off;
        }
        else {
            fprintf(stderr, "Error line %d: variable \'%s\' already declared in this scope\n", node->lineno, node->opr[0]->ident);
            exit(1);
        }
    }
    else {
        fprintf(stderr, "Error line %d: unknown declaration type \'%s\'\n", node->lineno, node_nature2string(node->nature));
        exit(1);
    }
}
