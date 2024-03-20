#include <stdio.h>

#include "defs.h"
#include "passe_1.h"
#include "miniccutils.h"

extern int trace_level;

void parcours_base(node_t node);
void analyse_declaration(bool global, node_t node);
void parcours_declaration(node_type type, bool global, node_t node);

void analyse_passe_1(node_t root) {
    push_global_context();
    analyse_declaration(1, root->opr[0]);
    parcours_base(root->opr[1]);
}
  
void parcours_base(node_t node) {
    if (node == NULL)
        return;

    if (node->nature == NODE_BLOCK) {
        push_context();
        analyse_declaration(0, node->opr[0]);
        // parcours_instructions(node->opr[1]); // TODO : implémenter
        pop_context();
    }
    else if (node->nature == NODE_FUNC) {
        reset_env_current_offset();
        push_context();
        // analyse_function(node);
        pop_context();
    }

    for(int i = 0; i < node->nops; i++) {
        parcours_base(node->opr[i]);
    }
}

void analyse_expression(node_t node) {

}

void analyse_instruction(node_t node) {

}

void analyse_declaration(bool global, node_t node) {
    if (node == NULL)
        return;
    if (node->nature == NODE_LIST) {
        analyse_declaration(global, node->opr[0]);
        analyse_declaration(global, node->opr[1]);
    }
    else {
        parcours_declaration(node->opr[0]->type, global, node->opr[1]);
    }
}

void parcours_declaration(node_type type, bool global, node_t node) {
    if (node->nature == NODE_LIST) {
        parcours_declaration(type, global, node->opr[0]);
        parcours_declaration(type, global, node->opr[1]);
    }
    else if (node->nature == NODE_DECLS) {
        parcours_declaration(node->opr[0]->type, global, node->opr[1]);
    }
    else {
        int off = env_add_element(node->opr[0]->ident, node->opr[0]);
        node->opr[0]->global_decl = global;
        node->opr[0]->type = type;

        if (off >= 0)
            node->opr[0]->offset = off;
        else
            node->opr[0]->offset = -1; // TODO : erreur à implémenter plus tard => déclaration préexistante
    }
}

/*void parcours_propre(node_t node) {
    if(node == NULL)
        return;
    traiter_noeud(node);
    for(int i; i < node->nops; i++) {
        parcours(node->opr[i]);
    }
}*/
