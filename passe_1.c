#include <stdio.h>

#include "defs.h"
#include "passe_1.h"

extern int trace_level;

void parcours_base(node_t node);
void analyse_instruction(node_t node);
void analyse_expression(node_t node);
void analyse_declaration(node_t node)



void analyse_passe_1(node_t root) {

    

}
  
void parcours_base(node_t node) {
    if(node == NULL)
        return;

    /*switch(node->nature) {
        case NODE_IDENT:
        case NODE_TYPE:
        case NODE_STRINGVAL:
        case NODE_INTVAL:
        case NODE_BOOLVAL:
            analyse_expression(node);
            break;
        default
            analyse_instruction(node);
            break;
    }*/

    for(int i; i < node->nops; i++) {
        parcours(node->opr[i]);
    }
}

void analyse_expression(node_t node) {
    switch(node->nature) {
        case 
    }
}

void analyse_instruction(node_t node) {

}

void analyse_declaration(node_t node) {

}

void parcours_declaration(node_type type, node_t node) {
    
}

/*void parcours_propre(node_t node) {
    if(node == NULL)
        return;
    traiter_noeud(node);
    for(int i; i < node->nops; i++) {
        parcours(node->opr[i]);
    }
}*/
