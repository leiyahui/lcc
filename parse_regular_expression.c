#include "lcc.h"
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



void change_atom_count_and_in_stack(int* atom_count, char** stack_head)
{
	*atom_count++;
	if (*atom_count > 1) {
		**stack_head = '.';
		(*stack_head)++;
		*atom_count--;
	}
}

/* 
'.' means concatation, 

1  between two letter should have one '.'
2  before '(' should have one '.'
3  after ')' should have one '.'
*/

static void trans_infix_to_postfix_expression(unsigned char* postfix_expr, unsigned char* infix_expr, int len)
{
	char op_stack[20], *op_top;
	int i, j, atom_count;

	op_top = op_stack;
	i = j = atom_count = 0;
	for (; i < len; i++) {
		if (is_letter(infix_expr[i]) == TRUE) {
			postfix_expr[j] = infix_expr[i];
			j++;

			change_atom_count_and_in_stack(&atom_count, &op_top);
		} else if (infix_expr[i] == '(') {
			change_atom_count_and_in_stack(&atom_count, &op_top);

			*op_top = '(';
			op_top++;
		} else if (infix_expr[i] == ')') {

			for (op_top--; *op_top != '('; op_top--) {
				postfix_expr[j] = *op_top;
				j++;
			}

			change_atom_count_and_in_stack(&atom_count, &op_top);
		} else if (infix_expr[i] == '*' || infix_expr[i] == '?') {
			postfix_expr[j] = infix_expr[i];
		} else {

		}
	}
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
	do {
		token = get_token(file->cursor);
		file->cursor++;
		length = get_regular_expression(file->cursor, &regular_expression);

		trans_infix_to_postfix_expression(postfix_expr, regular_expression, length);
		parse_one_regular_expression(token, regular_expression, length);
	} while(get_next_line(file) != NULL);
}