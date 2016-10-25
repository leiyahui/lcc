#include "lcc.h"

#define MAX_POST_FIX_EXPR_LEN 50

void init_expr_stack(operator_stack* stack)
{
	memset(stack->operators, 0, MAX_OPERATOR_NUM);
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
	stack->operators[stack->num] = op;
	stack->num++;
}

char out_oper_stack(operator_stack* stack)
{
	stack->num--;
	return stack->operators[stack->num];
}

char get_oper_stack(operator_stack* stack)
{
	return stack->operators[stack->num - 1];
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
	char c_operator;
	int j;
	if (*op == ')') {
		c_operator = out_oper_stack(stack);
		while (c_operator != '(') {
			**postfix_expr = c_operator;
			(*postfix_expr)++;
			c_operator = out_oper_stack(stack);
		}
	} else {
		if (!is_empty_oper_stack(stack)) {
			c_operator = get_oper_stack(stack);

			while (op_precede_cmp(c_operator, *op)) {
				out_oper_stack(stack);
				**postfix_expr = c_operator;
				(*postfix_expr)++;
				if (!is_empty_oper_stack(stack)) {
					c_operator = get_oper_stack(stack);
				} else {
					break;
				}
			}
		}
		in_oper_stack(stack, *op);
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
	int i, len_with_atom, postfix_len_with_atom;

	init_expr_stack(&stack);
	work_postfix_expr = postfix_expr;

	head_infix_expr_with_atom = infix_expr_with_atom = (unsigned char*)l_malloc(len * 2);
	postfix_len_with_atom = len_with_atom = add_atom_to_infix_expr(infix_expr_with_atom, infix_expr, len);
	log_debug("expression with atom %s\n", infix_expr_with_atom);

	for (i = 0; i < len_with_atom; i++) {
		if (is_letter(*infix_expr_with_atom)) {
			*work_postfix_expr = *infix_expr_with_atom;
			work_postfix_expr++;
		} else if (*infix_expr_with_atom == '(') {
			postfix_len_with_atom -= 2;
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
	while (!is_empty_oper_stack(&stack)) {
		*work_postfix_expr = out_oper_stack(&stack);
		work_postfix_expr++;
	}
	postfix_expr[postfix_len_with_atom] = '\0';
	log_debug("post fix expression %s\n", postfix_expr);

	l_free(head_infix_expr_with_atom);

	return postfix_len_with_atom;
}

state* parse_one_postfix_regular_expression(int token, unsigned char* regular_expression, int length)
{
	int i;
	char letter;
	frage_stack stack;
	frage frage;
	init_frage_stack(&stack);
	for (i = 0; i < length; i++) {
		letter = regular_expression[i];

		switch (letter) {
		case '.':
			concate_frage(&stack);
			break;
		case '|':
			union_frage(&stack);
			break;
		case '*':
			star_frage(&stack);
			break;
		default:
			create_frage(&stack, letter);
		}
	}
	out_frage_stack(&stack, &frage);

	return frage.start;

}

state_list* parse_regular_expression(input_file* file)
{
	int token, length, length_with_atom;
	unsigned char* postfix_expr;			//need to malloc memory
	unsigned char* regular_expression;
	state* s_state;
	state_list* start_state;

	postfix_expr = (unsigned char*)l_malloc(MAX_POST_FIX_EXPR_LEN);
	start_state = (state_list*)l_calloc(1, sizeof(state_list));

	do {
		token = get_token(file->cursor);
		file->cursor++;
		length = get_regular_expression(file->cursor, &regular_expression);

		length_with_atom = trans_infix_to_postfix_expression(postfix_expr, regular_expression, length);

		s_state = parse_one_postfix_regular_expression(token, postfix_expr, length_with_atom);
		add_state_to_list(start_state, s_state);
	} while(get_next_line(file) != NULL);

	return start_state;
}

void main()
{
	char* infix_expression = "a(a|b)d*";
	unsigned char* pos_fix_expression = (unsigned char*)l_malloc(30);
	int length_with_atom;
	state_list* start_state;
	state* s_state;

	init_logfile_fd(5);
	length_with_atom = trans_infix_to_postfix_expression(pos_fix_expression, infix_expression, 8);

	start_state = (state_list*)l_calloc(1, sizeof(state_list));

	s_state = parse_one_postfix_regular_expression(1, pos_fix_expression, length_with_atom);

	add_state_to_list(start_state, s_state);

	simulation_nfa(start_state, "hello", 6);


	printf("postfix expression is: %s\n", pos_fix_expression);
}