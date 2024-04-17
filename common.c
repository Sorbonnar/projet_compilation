
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <inttypes.h>
#include <unistd.h>
#include <getopt.h>

#include "defs.h"
#include "common.h"
#include "arch.h"


extern char * infile;
extern char * outfile;
int32_t trace_level = DEFAULT_TRACE_LEVEL;
extern bool stop_after_syntax;
extern bool stop_after_verif;

void affiche_utilisation() {
    printf("Usage: ./minicc [options] <source_file_to_compile>\n\n");
    printf("Options:\n");
    printf("  -b\t\tDisplay the compiler banner\n");
    printf("  -o <filename>\tSets outfile name (default : out.s)\n");
    printf("  -t <level>\tSets trace level between 0 and 5 (default : 0 = none; 5 = all)\n");
    printf("  -r <count>\tSets maximum number of registers to use between 4 and 8 (default : 8)\n");
    printf("  -s\t\tStop compiling after parsing (default : no)\n");
    printf("  -v\t\tStop compiling after the verifications pass (default : no)\n");
    printf("  -h\t\tDisplay help\n");
    printf("\nRules:\n");
    printf("  <source_file_to_compile> is required\n");
    printf("  Option -b is incompatible with every other options\n");
    printf("  Option -o must be followed by a filename\n");
    printf("  Trace level must be between 0 and 5\n");
    printf("  Number of registers must be between 4 and 8\n");
    printf("  Options -s and -v are incompatible\n");
    printf("\n");

    exit(1);
}

void parse_args(int argc, char **argv) {
    int arg_index = 1;
    bool b_option_used = false;
    int fichier_sortie = 0;

    while (argv[arg_index] != NULL) {
        char* arg = argv[arg_index];

        if (strcmp(arg, "-b") == 0) {
            if (argc > 2) {
                fprintf(stderr, "\nError: -b cannot be used with any other options.\n");
                exit(1);
            }
            printf("\n####################################################################\n");
            printf("####################################################################\n");
            printf("############################# COMPILAR #############################\n");
            printf("####################################################################\n");
            printf("################# MOHAMED GHAIBOUCHE - KARIM QORAR #################\n");
            printf("####################################################################\n");
            printf("####################################################################\n\n");
            exit(0);
        }

        arg_index++;

        if (strcmp(arg, "-o") == 0) {
            if (arg_index < argc) {
                outfile = strdupl(argv[arg_index]);
                arg_index++;
            }
            else {
                fprintf(stderr, "\nError: Missing argument for -o.\n\n");
                affiche_utilisation();
                exit(1);
            }
        }
        else if (strcmp(arg, "-t") == 0) {
            if (arg_index < argc) {
                char *argos = argv[arg_index];
                arg_index++;
                trace_level = atoi(argos);
                if (trace_level < 0 || trace_level > 5) {
                    fprintf(stderr, "\nError: Trace level must be between 0 and 5.\n\n");
                    affiche_utilisation();
                    exit(1);
                }
            }
            else {
                fprintf(stderr, "\nError: Missing argument for -t.\n\n");
                affiche_utilisation();
                exit(1);
            }
        }
        else if (strcmp(arg, "-r") == 0) {
            if (arg_index < argc) {
                char *argos = argv[arg_index];
                arg_index++;
                int max_regs = atoi(argos);
                if (max_regs < 4 || max_regs > 8) {
                    fprintf(stderr, "\nError: Number of registers must be between 4 and 8.\n\n");
                    affiche_utilisation();
                    exit(1);
                }
                set_max_registers(max_regs);
            }
            else {
                fprintf(stderr, "\nError: Missing argument for -r option.\n\n");
                affiche_utilisation();
                exit(1);
            }
        }
        else if (strcmp(arg, "-s") == 0) {
            if (stop_after_verif) {
                fprintf(stderr, "\nError: -s and -v options are incompatible.\n\n");
                affiche_utilisation();
                exit(1);
            }
            stop_after_syntax = true;
        }
        else if (strcmp(arg, "-v") == 0) {
            if (stop_after_syntax) {
                fprintf(stderr, "\nError: -s and -v options are incompatible.\n\n");
                affiche_utilisation();
                exit(1);
            }
            stop_after_verif = true;
        }
        else if (strcmp(arg, "-h") == 0) {
            affiche_utilisation();
            exit(0);
        }
        else if (strcmp(arg + strlen(arg) - 2, ".c") == 0 || strcmp(arg + strlen(arg) - 2, ".C") == 0) {
            if (fichier_sortie > 0) {
                fprintf(stderr, "\nError: Multiple outfiles specified.\n\n");
                affiche_utilisation();
                exit(1);
            }
            infile = arg;
            fichier_sortie++;
        }
        else {
            fprintf(stderr, "\nError: Unknown option \'%s\'.\n\n", arg);
            affiche_utilisation();
            exit(1);
        }
    }

    if (infile != NULL) {
        FILE * f = fopen(infile, "r");
        if (f == NULL) {
            fprintf(stderr, "\nError: Cannot open file \'%s\'.\n\n", infile);
            exit(1);
        }
        fclose(f);
    }
    else {
        fprintf(stderr, "\nError: Missing source file.\n\n");
        affiche_utilisation();
        exit(1);
    }
}

void free_nodes(node_t n) {
    if (n == NULL)
        return;

    for (int32_t i = 0; i < n->nops; i++) {
        free_nodes(n->opr[i]);
        n->opr[i] = NULL;
    }

    if (n->nops > 0) {
        free(n->opr);
        n->opr = NULL;
    }

    if (n->nature == NODE_IDENT && n->ident != NULL) {
        free(n->ident);
        n->ident = NULL;
    }
    else if (n->nature == NODE_STRINGVAL && n->str != NULL) {
        free(n->str);
        n->str = NULL;
    }

    free(n);
}



char * strdupl(char * s) {
    char * r = malloc(strlen(s) + 1);
    strcpy(r, s);
    return r;
}


static int32_t dump_tree2dot_rec(FILE * f, node_t n, int32_t node_num) {

    if (n == NULL) {
        fprintf(f, "    N%d [shape=record, label=\"{{NULL}}\"];\n", node_num);
        return node_num;
    }

    switch (n->nature) {
        case NODE_IDENT:
            {
                node_t decl_node = n->decl_node;
                fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}|{<decl>Decl      |Ident: %s|Offset: %d}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), n->ident, n->offset);
                if (decl_node != NULL && decl_node != n) {
                    fprintf(f, "    edge[tailclip=false];\n");
                    fprintf(f, "    \"N%d\":decl:c -> \"N%d\" [style=dashed]\n", node_num, decl_node->node_num);
                }
                break;
            }
        case NODE_INTVAL:
        case NODE_BOOLVAL:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}|{Value: %" PRId64 "}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), n->value);
            break;
        case NODE_STRINGVAL:
            {
                char str[32];
                int32_t i = 1;
                while (true) {
                    str[i - 1] = n->str[i];
                    i += 1;
                    if (n->str[i] == '"') {
                        str[i - 1] = '\0';
                        break;
                    }
                }
                fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}|{val: %s}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), str);
            }
            break;
        case NODE_TYPE:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type));
            break;
        case NODE_LIST:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE LIST}}\"];\n", node_num);
            break;
        case NODE_PROGRAM:
        case NODE_BLOCK:
        case NODE_DECLS:
        case NODE_DECL:
        case NODE_IF:
        case NODE_WHILE:
        case NODE_FOR:
        case NODE_DOWHILE:
        case NODE_PRINT:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Nb. ops: %d}}\"];\n", node_num, node_nature2string(n->nature), n->nops);
            break;
        case NODE_FUNC:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Nb. ops: %d}|{offset: %d}}\"];\n", node_num, node_nature2string(n->nature), n->nops, n->offset);
            break;
        case NODE_PLUS:
        case NODE_MINUS:
        case NODE_MUL:
        case NODE_DIV:
        case NODE_MOD:
        case NODE_LT:
        case NODE_GT:
        case NODE_LE:
        case NODE_GE:
        case NODE_EQ:
        case NODE_NE:
        case NODE_AND:
        case NODE_OR:
        case NODE_BAND:
        case NODE_BOR:
        case NODE_BXOR:
        case NODE_SRA:
        case NODE_SRL:
        case NODE_SLL:
        case NODE_NOT:
        case NODE_BNOT:
        case NODE_UMINUS:
        case NODE_AFFECT:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s|Nb. ops: %d}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), n->nops);
            break;
        default:
            printf("*** Error in %s: unknow nature : %s\n", __func__, node_nature2string(n->nature));
            assert(false);
    }

    n->node_num = node_num;

    int32_t curr_node_num = node_num + 1;
    for (int32_t i = 0; i < n->nops; i += 1) {
        int32_t new_node_num = dump_tree2dot_rec(f, n->opr[i], curr_node_num);

        fprintf(f, "    edge[tailclip=true];\n");
        fprintf(f, "    N%d -> N%d\n", node_num, curr_node_num);
        curr_node_num = new_node_num + 1;
    }

    return curr_node_num - 1;
}



static void dump_tree2dot(FILE * f, node_t root) {
    assert(root->nature == NODE_PROGRAM);

    int32_t curr_node_num = 1;
    dump_tree2dot_rec(f, root, curr_node_num);
}


void dump_tree(node_t prog_root, const char * dotname) {

    FILE * f;

    f = fopen(dotname, "w");
    fprintf(f, "digraph global_vars {\n");
    dump_tree2dot(f, prog_root);
    fprintf(f, "}");    
    fclose(f);
}


const char * node_type2string(node_type t) {
    switch (t) {
        case TYPE_NONE:
            return "TYPE NONE";
        case TYPE_INT:
            return "TYPE INT";
        case TYPE_BOOL:
            return "TYPE BOOL";
        case TYPE_VOID:
            return "TYPE VOID";
        default:
            assert(false);
    }
}


const char * node_nature2string(node_nature t) {
    switch (t) {
        case NONE:
            return "NONE";
        case NODE_PROGRAM:
            return "PROGRAM";
        case NODE_BLOCK:
            return "BLOCK";
        case NODE_LIST:
            return "LIST";
        case NODE_DECLS:
            return "DECLS";
        case NODE_DECL:
            return "DECL";
        case NODE_IDENT:
            return "IDENT";
        case NODE_TYPE:
            return "TYPE";
        case NODE_INTVAL:
            return "INTVAL";
        case NODE_BOOLVAL:
            return "BOOLVAL";
        case NODE_STRINGVAL:
            return "STRINGVAL";
        case NODE_FUNC:
            return "FUNC";
        case NODE_IF:
            return "IF";
        case NODE_WHILE:
            return "WHILE";
        case NODE_FOR:
            return "FOR";
        case NODE_DOWHILE:
            return "DOWHILE";
        case NODE_PLUS:
            return "PLUS";
        case NODE_MINUS:
            return "MINUS";
        case NODE_MUL:
            return "MUL";
        case NODE_DIV:
            return "DIV";
        case NODE_MOD:
            return "MOD";
        case NODE_LT:
            return "LT";
        case NODE_GT:
            return "GT";
        case NODE_LE:
            return "LE";
        case NODE_GE:
            return "GE";
        case NODE_EQ:
            return "EQ";
        case NODE_NE:
            return "NE";
        case NODE_AND:
            return "AND";
        case NODE_OR:
            return "OR";
        case NODE_BAND:
            return "BAND";
        case NODE_BOR:
            return "BOR";
        case NODE_BXOR:
            return "BXOR";
        case NODE_SLL:
            return "SLL";
        case NODE_SRA:
            return "SRA";
        case NODE_SRL:
            return "SRL";
        case NODE_NOT:
            return "NOT";
        case NODE_BNOT:
            return "BNOT";
        case NODE_UMINUS:
            return "UMINUS";
        case NODE_AFFECT:
            return "AFFECT";
        case NODE_PRINT:
            return "PRINT";
        default:
            fprintf(stderr, "*** Error in %s: Unknown node nature: %d\n", __func__, t);
            exit(1);
    }
}




const char * node_nature2symb(node_nature t) {
    switch (t) {
        case NONE:
        case NODE_PLUS:
            return "+";
        case NODE_MINUS:
            return "-";
        case NODE_MUL:
            return "*";
        case NODE_DIV:
            return "/";
        case NODE_MOD:
            return "%";
        case NODE_LT:
            return "<";
        case NODE_GT:
            return ">";
        case NODE_LE:
            return "<=";
        case NODE_GE:
            return ">=";
        case NODE_EQ:
            return "==";
        case NODE_NE:
            return "!=";
        case NODE_AND:
            return "&&";
        case NODE_OR:
            return "||";
        case NODE_BAND:
            return "&";
        case NODE_BOR:
            return "|";
        case NODE_BXOR:
            return "^";
        case NODE_SRA:
            return ">>";
        case NODE_SRL:
            return ">>>";
        case NODE_SLL:
            return "<<";
        case NODE_NOT:
            return "!";
        case NODE_BNOT:
            return "~";
        case NODE_UMINUS:
            return "-";
        default:
            fprintf(stderr, "*** Error in %s: Unknown node nature: %d\n", __func__, t);
            exit(1);
    }
}
