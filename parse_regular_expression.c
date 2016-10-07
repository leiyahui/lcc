#include "lcc.h"
nfa_state nfa;

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
	do {
		token = *(file->cursor);
		while(*(file->cursor) == ' ' || *(file->cursor) == '\t') {			//rm space
			file->cursor++;
		}
		while()
		parse_one_regular_expression(token, file->cursor, )
	}
}