#ifndef _PARSE_REGULAR_EXPRESSION_H
#define _PARSE_REGULAR_EXPRESSION_H
#include "lcc.h"

extern nfa_state nfa;

void parse_regular_expression(int token, char* regular_expression, int length);

#endif