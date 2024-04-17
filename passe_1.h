#ifndef _PASSE_1_
#define _PASSE_1_

#include "defs.h"

void analyse_passe_1(node_t root);
void analyse_base(node_t node);
void analyse_expression(node_t node);
void analyse_instruction(node_t node);
void analyse_declaration(bool global, node_t node, node_type type);
node_t make_node(node_nature nature, int nops, ...);

#endif

