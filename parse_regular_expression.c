#include "lcc.h"

#define MAX_POST_FIX_EXPR_LEN 50

void init_expr_stack(operator_stack* stack)
{
	memset(stack->operator, 0, MAX_OPERATOR_NUM);
	stack->num = 0;
}

BOOL is_empty_oper_stack(operator_stack* stack)
{
	if (stack->num == 0) {
		return TRUE;
	}
	return FALSE;
}

BOOL is_full_oper_stack(operator_stack* stack)
{
	if (stack->num == MAX_OPERATOR_NUM) {
		return TRUE;
	}
	return FALSE;
}

void in_oper_stack(operator_stack* stack, char op)
{
	stack->operator[stack->num] = op;
	stack->num++;
}

char out_oper_stack(operator_stack* stack)
{
	stack->num--;
	return stack->operator[stack->num];
}

char get_oper_stack(operator_stack* stack)
{
	return stack->operator[stack->num - 1];
}

static int get_token(unsigned char* current_cursor)
{
	int token;
	token = *current_cursor;
	return token;
}

static int get_regular_expression(unsigned char* current_cursor, unsigned char** regular_expression)		//return the regular expression length
{
	int length = 0;

	while(*current_cursor == ' ' || *current_cursor == '\t') {			//rm space
		current_cursor++;
	}

	*regular_expression = current_cursor;

	while (*current_cursor != ' ' && *current_cursor != '\t' && *current_cursor != '\n') {
		length++;
	}
	return length;
}

static BOOL op_precede_cmp(unsigned char op1, unsigned char op2)
{
	if (op1 == op2 || op1 == '.' && op2 == '|') {
		return TRUE;
	}
	return FALSE;
}

void retrieve_low_precedence_letter(operator_stack* stack,  unsigned char** postfix_expr, unsigned char* op)
{
	char operator;
	if (*op == ')') {
		operator = out_oper_stack(stack);
		while (operator != '(') {
			**postfix_expr = operator;
			*postfix_expr++;
			operator = out_oper_stack(stack);
		}
	} else {
		if (!is_empty_oper_stack(stack)) {
			operator = get_oper_stack(stack);

			while (op_precede_cmp(operator, *op)) {
				out_oper_stack(stack);
				**postfix_expr = operator;
				*postfix_expr++;
				if (!is_empty_oper_stack(stack)) {
					operator = get_oper_stack(stack);
				} else {
					break;
				}
			}
		}
		in_oper_stack(stack, *op);
		for (int j = 0; j < stack->num; j++) {
			log_debug("%c, ", stack->operator[j]);
		}
		log_debug("\n");
	}
}

int add_atom_to_infix_expr(unsigned char* infix_expr_with_atom, unsigned char* infix_expr, int length)
{
	unsigned char letter_l, letter_r, *work_infix_expr_with_atom, *work_infix_expr;
	int i, len_with_atom;

	work_infix_expr = infix_expr;
	work_infix_expr_with_atom = infix_expr_with_atom;
	len_with_atom = 0;

	for (i = 0; i < length; i++) {
		letter_l = *work_infix_expr;
		letter_r = *(work_infix_expr + 1);

		if (letter_l == '|' || letter_l == '*' || letter_l == '?' || letter_r == '|'
			 || letter_r == '*' || letter_r == '?' || letter_l == '(' || letter_r == ')') {
			*work_infix_expr_with_atom = *work_infix_expr;
			work_infix_expr_with_atom++;
			len_with_atom++;
		} else {
			*work_infix_expr_with_atom = *work_infix_expr;
			work_infix_expr_with_atom++;
			*work_infix_expr_with_atom = '.';
			work_infix_expr_with_atom++;
			len_with_atom += 2;
		}

		work_infix_expr++;

	}
	infix_expr_with_atom[len_with_atom] = '\0';

	return len_with_atom;
}

/*
'.' means concatation,

1  between two letter should have one '.'
2  before '(' should have one '.'
3  after ')' should have one '.'
*/

static int trans_infix_to_postfix_expression(unsigned char* postfix_expr, unsigned char* infix_expr, int len)
{
	operator_stack stack;
	unsigned char *head_infix_expr_with_atom, *infix_expr_with_atom, *work_postfix_expr;
	int i, len_with_atom;

	init_expr_stack(&stack);
	work_postfix_expr = postfix_expr;

	head_infix_expr_with_atom = infix_expr_with_atom = (unsigned char*)l_malloc(len * 2);
	len_with_atom = add_atom_to_infix_expr(infix_expr_with_atom, infix_expr, len);
	log_debug("expression with atom %s\n", infix_expr_with_atom);

	for (i = 0; i < len_with_atom; i++) {
		if (is_letter(*infix_expr_with_atom)) {
			*work_postfix_expr = *infix_expr_with_atom;
			work_postfix_expr++;
		} else if (*infix_expr_with_atom == '(') {
			if (!is_full_oper_stack(&stack)) {
				in_oper_stack(&stack, '(');
			} else {
				log_error("operator stack full\n");
				exit(0);
			}
		} else if (*infix_expr_with_atom == '*' || *infix_expr_with_atom == '?') {
			*work_postfix_expr = *infix_expr_with_atom;
			work_postfix_expr++;
		} else {
			retrieve_low_precedence_letter(&stack, &work_postfix_expr, infix_expr_with_atom);
		}

		infix_expr_with_atom++;
	}
	log_debug("post fix expression %s\n", postfix_expr);

	l_free(head_infix_expr_with_atom);

	return len_with_atom;
}

void main()
{
	char* infix_expression = "abc(a|b)d*";
	char* pos_fix_expression = (char*)l_malloc(30);
	init_logfile_fd(5);
	trans_infix_to_postfix_expression(pos_fix_expression, infix_expression, 10);
	printf("postfix expression is: %s\n", pos_fix_expression);
}

void parse_one_postfix_regular_expression(int token, unsigned char* regular_expression, int length)
{
	int i;
	char letter;
	frage_stack stack;
	for (i = 0; i < length; i++) {
		letter = regular_expression[i];

		switch (letter) {
		case '.':
			concate_frage(&stack);
		case '|':
			union_frage(&stack);
		case '*':
			star_frage(&stack);
		default:
			create_frage(&stack, letter);
		}
	}
}

void parse_regular_expression(input_file* file)
{
	int token, length;
	unsigned char* postfix_expr;			//need to malloc memory
	unsigned char* regular_expression;

	postfix_expr = (unsigned char*)l_malloc(MAX_POST_FIX_EXPR_LEN);

	do {
		token = get_token(file->cursor);
		file->cursor++;
		length = get_regular_expression(file->cursor, &regular_expression);

		trans_infix_to_postfix_expression(postfix_expr, regular_expression, length);

		parse_one_postfix_regular_expression(token, postfix_expr, length);
	} while(get_next_line(file) != NULL);
}

