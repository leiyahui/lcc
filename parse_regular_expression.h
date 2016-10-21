#ifndef _PARSE_REGULAR_EXPRESSION_H
#define _PARSE_REGULAR_EXPRESSION_H
#include "lcc.h"
#define MAX_OPERATOR_NUM 30
typedef struct _operator_stack {
	char operator[MAX_OPERATOR_NUM];
	int num;
}operator_stack;

void init_expr_stack(operator_stack* stack);

BOOL is_empty_oper_stack(operator_stack* stack);

BOOL is_full_oper_stack(operator_stack* stack);

void in_oper_stack(operator_stack* stack, char op);

char out_oper_stack(operator_stack* stack);

char get_oper_stack(operator_stack* stack);


void parse_regular_expression(input_file* file);

void parse_one_postfix_regular_expression(int token, unsigned char* regular_expression, int length);


#endif