#include "lcc.h"

#define MAX_POST_FIX_EXPR_LEN 50

nfa_state nfa;

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

static BOOL op_precede_cmp(char* op1, char* op2)
{
	if (*op1 == *op2 || *op1 == '.' && *op2 == '|') {
		return TRUE;
	}
	return FALSE;
}

void retrieve_low_precedence_letter(char** stack_head, unsigned char** postfix_expr, char* op)
{
	if (*op == '(') {
		**stack_head = *op;
		*stack_head++;
		return;
	} else if (*op == ')') {
		*stack_head--;
		while (**stack_head != '(') {
			**postfix_expr = **stack_head;
			*postfix_expr++;
			*stack_head--;
		}
	} else {
		*stack_head--;
		while (op_precede_cmp(*stack_head, op)) {
			**postfix_expr = **stack_head;
			*postfix_expr++;
			*stack_head--;
		}
		*stack_head++;
	}
}

int add_atom_to_infix_expr(char* infix_expr_with_atom, char* infix_expr, int length)
{
	char letter_l, letter_r, *work_infix_expr_with_atom, *work_infix_expr;
	int i, len_with_atom;

	work_infix_expr = infix_expr;
	work_infix_expr_with_atom = infix_expr_with_atom;
	len_with_atom = 0;

	for (i = 0; i < length; i++) {
		letter_l = *work_infix_expr;
		letter_r = *(work_infix_expr++);

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
	char* stack, *infix_expr_with_atom, *work_postfix_expr;
	int i, len_with_atom;

	stack = (char*)l_malloc(len * 2);
	work_postfix_expr = postfix_expr;

	infix_expr_with_atom = (char*)l_malloc(len * 2);
	len_with_atom = add_atom_to_infix_expr(infix_expr_with_atom, infix_expr, len);

	for (i = 0; i < len_with_atom; i++) {
		if (is_letter(*infix_expr_with_atom)) {
			*postfix_expr = *infix_expr_with_atom;
			postfix_expr++;
		} else if (*infix_expr_with_atom == '(') {
			*stack = '(';
			stack++;
		} else if (*infix_expr_with_atom == '*' || *infix_expr_with_atom == '?') {
			*postfix_expr = *infix_expr_with_atom;
			postfix_expr++;
		} else {
			retrieve_low_precedence_letter(&stack, &postfix_expr, infix_expr_with_atom);
		}

		infix_expr_with_atom++;
	}

	l_free(stack);
	l_free(infix_expr_with_atom);

	return len_with_atom;
}

void parse_one_regular_expression(int token, unsigned char* regular_expression, int length)
{
	int i;
	nfa_state* from_state, *to_state;

	from_state = &nfa;
	for (i = 0; i < length; i++) {
		to_state = (nfa_state*)l_malloc(sizeof(nfa_state));
		init_nfa_state(to_state);
		add_next_state(from_state, to_state, regular_expression[i]);
		from_state = to_state;
	}
	add_accepting_token(from_state, token);
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

		parse_one_regular_expression(token, postfix_expr, length);
	} while(get_next_line(file) != NULL);
}