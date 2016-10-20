#ifndef _PARSE_REGULAR_EXPRESSION_H
#define _PARSE_REGULAR_EXPRESSION_H
#include "lcc.h"
#define MAX_OPERATOR_NUM 30
typedef struct _operator_stack {
	char operator[MAX_OPERATOR_NUM];
	int num;
}operator_stack;

void parse_regular_expression(input_file* file);

void parse_one_postfix_regular_expression(int token, unsigned char* regular_expression, int length);


#endif