#ifndef _PASSE_1_
#define _PASSE_1_

#include "defs.h"

void parcours_base(node_t node);
void process_expression(node_t node);
void process_instruction(node_t node);
void process_declaration(bool global, node_t node, node_type type);
void parcours_declaration(node_type type, bool global, node_t node);

#endif

