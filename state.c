#include "lcc.h"
void init_nfa_state(nfa_state* state)
{
	nfa_state_one_letter* one_letter = state->letter;
	one_letter = (nfa_state_one_letter*)l_calloc(sizeof(nfa_state_one_letter) * DEFAULT_LETTER_COUNT);
}

void init_one_letter(nfa_state_one_letter *letter) 
{
	nfa_state** state = letter->next_state;
	state = (nfa_state**)l_calloc(sizeof(nfa_state*) * DEFAULT_LETTER_NEXT_STATE);
}

void add_next_state(nfa_state *from_state, nfa_state *to_state, char letter)
{
	int i;
	nfa_state_one_letter *one_letter = from_state->letter;
	nfa_state* next_state;
	for (i = 0; i < from_state->letter_num; i++) {
		if (one_letter[i].letter == letter){
			one_letter[i].next_state[one_letter[i].state_num] = to_state;
			one_letter[i].state_num++;
			break;
		} else {
			init_one_letter(one_letter);

			one_letter = (nfa_state_one_letter*)(one_letter + sizeof(nfa_state_one_letter) * from_state->letter_num);
			from_state->letter_num++;

			one_letter->letter = letter;
			one_letter->next_state[one_letter->state_num] = to_state;
			one_letter->state_num++;
		}
	}
}


void free_nfa_state(nfa_state* state) 
{
	int i;
	nfa_state_one_letter* one_letter = state->letter;
	for(i = 0; i < state->letter_num; i++) {
		free_one_letter(one_letter + sizeof(nfa_state_one_letter) * i);
	}

	if (state->is_acc_stat) {
		l_free(state->acc_token);
	}
	l_free(one_letter);
}

void free_one_letter(nfa_state_one_letter* letter)
{
	l_free(letter->next_state);
}

void add_accepting_token(nfa_state* state, int token) {
	state->is_acc_stat =1;
	state->acc_token = (accept_toekn*)l_calloc(sizeof(accept_toekn));
	state->acc_token->token = token;
}