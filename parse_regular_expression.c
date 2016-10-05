#include "lcc.h"
nfa_state nfa;

void parse_regular_expression(int token, char* regular_expression, int length)
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
}