%{
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "defs.h"
#include "common.h"
#include "miniccutils.h"
#include "passe_1.h"
#include "passe_2.h"

/* Global variables */
extern bool stop_after_syntax;
extern bool stop_after_verif;
extern char * outfile;
extern int trace_level;

/* prototypes */
int yylex(void);
extern int yylineno;

void yyerror(node_t * program_root, char * s);
void analyse_tree(node_t root);
node_t make_node(node_nature nature, int nops, ...);
node_t make_terminal_node(node_nature nature, ...);

%}

%parse-param { node_t * program_root }

%union {
    int32_t intval;
    char * strval;
    node_t ptr;
};

%token TOK_VOID TOK_INT TOK_BOOL TOK_TRUE TOK_FALSE TOK_IF TOK_DO TOK_WHILE TOK_FOR
%token TOK_PRINT TOK_SEMICOL TOK_COMMA TOK_LPAR TOK_RPAR TOK_LACC TOK_RACC
%token <intval> TOK_INTVAL
%token <strval> TOK_IDENT TOK_STRING

%nonassoc TOK_THEN
%nonassoc TOK_ELSE

%right TOK_AFFECT

%left TOK_OR
%left TOK_AND
%left TOK_BOR
%left TOK_BXOR
%left TOK_BAND
%nonassoc TOK_EQ TOK_NE
%nonassoc TOK_GT TOK_LT TOK_GE TOK_LE
%nonassoc TOK_SRL TOK_SRA TOK_SLL

%left TOK_PLUS TOK_MINUS
%left TOK_MUL TOK_DIV TOK_MOD

%nonassoc TOK_UMINUS TOK_NOT TOK_BNOT

%type <ptr> program listdecl listdeclnonnull vardecl ident type listtypedecl decl maindecl
%type <ptr> listinst listinstnonnull inst block expr listparamprint paramprint

%%

program:
        listdeclnonnull maindecl
        {
            printf_level(5, "program rule : Global declarations\n");
            $$ = make_node(NODE_PROGRAM, 2, $1, $2);
            *program_root = $$;
        }
        | maindecl
        {
            printf_level(5, "program rule : No global declarations\n");
            $$ = make_node(NODE_PROGRAM, 2, NULL, $1);
            *program_root = $$;
        }
        ;

listdecl:
        listdeclnonnull
        {
            printf_level(5, "listdecl rule : ");
            $$ = $1;
        }
        |
        { 
            $$ = NULL;
        }
        ;

listdeclnonnull:
        vardecl
        { 
            printf_level(5, "listdeclnonnull rule : ");
            $$ = $1;
        }
        | listdeclnonnull vardecl
        { 
            printf_level(5, "listdeclnonnull rule : Several declarations\n");
            $$ = make_node(NODE_LIST, 2, $1, $2);
        }
        ;

vardecl:
        type listtypedecl TOK_SEMICOL
        { 
            printf_level(5, "vardecl rule : Variable declaration\n");
            $$ = make_node(NODE_DECLS, 2, $1, $2);
        }
        ;

listtypedecl:
        decl
        { 
            printf_level(5, "listtypedecl rule : ");
            $$ = $1;
        }
        | listtypedecl TOK_COMMA decl
        { 
            printf_level(5, "listtypedecl rule : Several declarations\n");
            $$ = make_node(NODE_LIST, 2, $1, $3);
        }
        ;

decl:
        ident
        { 
            printf_level(5, "decl rule : NULL declaration\n");
            $$ = make_node(NODE_DECL, 2, $1, NULL);
        }
        | ident TOK_AFFECT expr
        { 
            printf_level(5, "decl rule : Declaration with affectation\n");
            $$ = make_node(NODE_DECL, 2, $1, $3);
        }
        ;

maindecl:
        type ident TOK_LPAR TOK_RPAR block
        { 
            printf_level(5, "maindecl rule : main function\n");
            $$ = make_node(NODE_FUNC, 3, $1, $2, $5);
        }
        ;

type:
        TOK_INT
        { 
            printf_level(5, "type rule : Int\n");
            $$ = make_terminal_node(NODE_TYPE, 0);
        }
        | TOK_BOOL
        { 
            printf_level(5, "type rule : Bool\n");
            $$ = make_terminal_node(NODE_TYPE, 1);
        }
        | TOK_VOID
        { 
            printf_level(5, "type rule : Void\n");
            $$ = make_terminal_node(NODE_TYPE, 2);
        }
        ;

listinst:
        listinstnonnull
        { 
            printf_level(5, "listinst rule : ");
            $$ = $1;
        }
        |
        { 
            $$ = NULL;
        }
        ;

listinstnonnull:
        inst
        { 
            printf_level(5, "listinstnonnull rule : ");
            $$ = $1;
        }
        | listinstnonnull inst
        { 
            printf_level(5, "listinstnonnull rule : Several instructions\n");
            $$ = make_node(NODE_LIST, 2, $1, $2);
        }
        ;

inst:
        expr TOK_SEMICOL
        {
            printf_level(5, "inst rule : ");
            $$ = $1;
        }
        | TOK_IF TOK_LPAR expr TOK_RPAR inst TOK_ELSE inst
        { 
            printf_level(5, "inst rule : If else\n");
            $$ = make_node(NODE_IF, 3, $3, $5, $7);
        }
        | TOK_IF TOK_LPAR expr TOK_RPAR inst %prec TOK_THEN
        { 
            printf_level(5, "inst rule : If\n");
            $$ = make_node(NODE_IF, 2, $3, $5);
        }
        | TOK_WHILE TOK_LPAR expr TOK_RPAR inst
        { 
            printf_level(5, "inst rule : While\n");
            $$ = make_node(NODE_WHILE, 2, $3, $5);
        }
        | TOK_FOR TOK_LPAR expr TOK_SEMICOL expr TOK_SEMICOL expr TOK_RPAR inst
        { 
            printf_level(5, "inst rule : For\n");
            $$ = make_node(NODE_FOR, 4, $3, $5, $7, $9);
        }
        | TOK_DO inst TOK_WHILE TOK_LPAR expr TOK_RPAR TOK_SEMICOL
        { 
            printf_level(5, "inst rule : Do while\n");
            $$ = make_node(NODE_DOWHILE, 2, $2, $5);
        }
        | block
        { 
            printf_level(5, "inst rule : ");
            $$ = $1;
        }
        | TOK_SEMICOL
        { 
            $$ = NULL;
        }
        | TOK_PRINT TOK_LPAR listparamprint TOK_RPAR TOK_SEMICOL
        { 
            printf_level(5, "inst rule : Print\n");
            $$ = make_node(NODE_PRINT, 1, $3);
        }
        ;

block:
        TOK_LACC listdecl listinst TOK_RACC
        {
            printf_level(5, "block rule : Block\n");
            $$ = make_node(NODE_BLOCK, 2, $2, $3);
        }
        ;

expr:
        expr TOK_MUL expr
        {
            printf_level(5, "expr rule : Multiplication\n");
            $$ = make_node(NODE_MUL, 2, $1, $3);
        }
        | expr TOK_DIV expr
        { 
            printf_level(5, "expr rule : Division\n");
            $$ = make_node(NODE_DIV, 2, $1, $3);
        }
        | expr TOK_PLUS expr
        { 
            printf_level(5, "expr rule : Addition\n");
            $$ = make_node(NODE_PLUS, 2, $1, $3);
        }
        | expr TOK_MINUS expr
        { 
            printf_level(5, "expr rule : Subtraction\n");
            $$ = make_node(NODE_MINUS, 2, $1, $3);
        }
        | expr TOK_MOD expr
        { 
            printf_level(5, "expr rule : Modulo\n");
            $$ = make_node(NODE_MOD, 2, $1, $3);
        }
        | expr TOK_LT expr
        { 
            printf_level(5, "expr rule : Less than\n");
            $$ = make_node(NODE_LT, 2, $1, $3);
        }
        | expr TOK_GT expr
        { 
            printf_level(5, "expr rule : Greater than\n");
            $$ = make_node(NODE_GT, 2, $1, $3);
        }
        | TOK_MINUS expr %prec TOK_UMINUS
        { 
            printf_level(5, "expr rule : Unary minus\n");
            $$ = make_node(NODE_UMINUS, 1, $2);
        }
        | expr TOK_GE expr
        { 
            printf_level(5, "expr rule : Greater or equal\n");
            $$ = make_node(NODE_GE, 2, $1, $3);
        }
        | expr TOK_LE expr
        { 
            printf_level(5, "expr rule : Less or equal\n");
            $$ = make_node(NODE_LE, 2, $1, $3);
        }
        | expr TOK_EQ expr
        { 
            printf_level(5, "expr rule : Equal\n");
            $$ = make_node(NODE_EQ, 2, $1, $3);
        }
        | expr TOK_NE expr
        { 
            printf_level(5, "expr rule : Not equal\n");
            $$ = make_node(NODE_NE, 2, $1, $3);
        }
        | expr TOK_AND expr
        { 
            printf_level(5, "expr rule : And\n");
            $$ = make_node(NODE_AND, 2, $1, $3);
        }
        | expr TOK_OR expr
        { 
            printf_level(5, "expr rule : Or\n");
            $$ = make_node(NODE_OR, 2, $1, $3);
        }
        | expr TOK_BAND expr
        { 
            printf_level(5, "expr rule : Band\n");
            $$ = make_node(NODE_BAND, 2, $1, $3);
        }
        | expr TOK_BOR expr
        { 
            printf_level(5, "expr rule : Bor\n");
            $$ = make_node(NODE_BOR, 2, $1, $3);
        }
        | expr TOK_BXOR expr
        { 
            printf_level(5, "expr rule : Bxor\n");
            $$ = make_node(NODE_BXOR, 2, $1, $3);
        }
        | expr TOK_SRL expr
        { 
            printf_level(5, "expr rule : Shift right logical\n");
            $$ = make_node(NODE_SRL, 2, $1, $3);
        }
        | expr TOK_SRA expr
        { 
            printf_level(5, "expr rule : Shift right arithmetic\n");
            $$ = make_node(NODE_SRA, 2, $1, $3);
        }
        | expr TOK_SLL expr
        { 
            printf_level(5, "expr rule : Shift left logical\n");
            $$ = make_node(NODE_SLL, 2, $1, $3);
        }
        | TOK_NOT expr
        { 
            printf_level(5, "expr rule : Not\n");
            $$ = make_node(NODE_NOT, 1, $2);
        }
        | TOK_BNOT expr
        { 
            printf_level(5, "expr rule : Bnot\n");
            $$ = make_node(NODE_BNOT, 1, $2);
        }
        | TOK_LPAR expr TOK_RPAR
        { 
            printf_level(5, "expr rule : Parenthesis\n");
            $$ = $2;
        }
        | ident TOK_AFFECT expr
        {
            printf_level(5, "expr rule : Affectation\n");
            $$ = make_node(NODE_AFFECT, 2, $1, $3);
        }
        | TOK_INTVAL
        { 
            printf_level(5, "expr rule : Int value\n");
            $$ = make_terminal_node(NODE_INTVAL, $1);
        }
        | TOK_TRUE
        { 
            printf_level(5, "expr rule : Bool value\n");
            $$ = make_terminal_node(NODE_BOOLVAL, 1);
        }
        | TOK_FALSE
        { 
            printf_level(5, "expr rule : Bool value\n");
            $$ = make_terminal_node(NODE_BOOLVAL, 0);
        }
        | ident
        { 
            printf_level(5, "expr rule : ");
            $$ = $1;
        }
        ;

listparamprint:
        listparamprint TOK_COMMA paramprint
        { 
            printf_level(5, "listparamprint rule : Several parameters\n");
            $$ = make_node(NODE_LIST, 2, $1, $3);
        }
        | paramprint
        { 
            printf_level(5, "listparamprint rule : ");
            $$ = $1;
        }
        ;

paramprint:
        ident
        { 
            printf_level(5, "paramprint rule : ");
            $$ = $1;
        }
        | TOK_STRING
        { 
            printf_level(5, "paramprint rule : String\n");
            $$ = make_terminal_node(NODE_STRINGVAL, $1);
        }
        ;

ident:
        TOK_IDENT
        { 
            printf_level(5, "ident rule : Identifier\n");
            $$ = make_terminal_node(NODE_IDENT, $1);
        }
        ;

%%

node_t make_node(node_nature nature, int nops, ...) {
    node_t node = (node_t) malloc(sizeof(node_s));
    if (node == NULL) {
        fprintf(stderr, "Error line %d: malloc error\n", yylineno);
        exit(1);
    }

    node->nature = nature;
    node->lineno = yylineno;
    node->nops = nops;
    node->global_decl = false;
    node->type = TYPE_NONE;
    node->value = -1;
    node->str = NULL;
    node->ident = NULL;
    node->offset = -1;
    node->decl_node = NULL;
    node->node_num = -1;

    if (nops > 0) {
        node->opr = (node_t*) malloc(nops * sizeof(node_t));
        if (node->opr == NULL) {
            free(node);
            fprintf(stderr, "Error line %d: malloc error\n", yylineno);
            exit(1);
        }
    } else {
        node->opr = NULL;
    }

    va_list ap;
    va_start(ap, nops);
    for (int i = 0; i < nops; i++) {
        node->opr[i] = va_arg(ap, node_t);
    }
    va_end(ap);

    return node;
}


node_t make_terminal_node(node_nature nature, ...) {
    node_t node = make_node(nature, 0);

    int intval;
    char* strval;
    
    if (node == NULL) {
        fprintf(stderr, "Error line %d: malloc error\n", yylineno);
        exit(1);
    }

    va_list args;
    va_start(args, nature);

    switch (nature) {
        case NODE_IDENT:
        case NODE_STRINGVAL:
            strval = va_arg(args, char*);
            
            if (strval == NULL) {
                free(node);
                fprintf(stderr, "Error line %d: NULL string error\n", yylineno);
                exit(1);
            }

            if (nature == NODE_IDENT) {
                node->ident = strdupl(strval);
            }
            else {
                node->str = strdupl(strval);
            }
            free(strval);
            break;
        
        case NODE_TYPE:
            intval = va_arg(args, int);
            node->type = (intval == 0) ? TYPE_INT : (intval == 1) ? TYPE_BOOL : (intval == 2) ? TYPE_VOID : TYPE_NONE;
            break;

        case NODE_INTVAL:
        case NODE_BOOLVAL:
            node->value = va_arg(args, int);
            break;
            
        default:
            va_end(args);
            free(node);
            fprintf(stderr, "Error line %d: Unknown node nature error\n", yylineno);
            exit(1);
            break;
    }

    va_end(args);
    return node;
}


void analyse_tree(node_t root) {
    dump_tree(root, "apres_syntaxe.dot");
    if (!stop_after_syntax) {
        analyse_passe_1(root);
        dump_tree(root, "apres_passe_1.dot");
        if (!stop_after_verif) {
            create_program(); 
            gen_code_passe_2(root);
            dump_mips_program(outfile);
            free_program();
        }
        free_global_strings();
    }
    free_nodes(root);
}


/*
 * Cette fonction est appelee automatiquement si une erreur de syntaxe est rencontree
 * N'appelez pas cette fonction vous-meme :    
 * la valeur donnee par yylineno ne sera plus correcte apres l'analyse syntaxique
 */
void yyerror(node_t * program_root, char * s) {
    fprintf(stderr, "Error line %d: %s\n", yylineno, s);
    exit(1);
}
