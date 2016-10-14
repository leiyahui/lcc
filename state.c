/*dfa's or nfa's state*/
#include "lcc.h"
//void init_nfa_state(nfa_state* state)
//{
//	nfa_state_one_letter* one_letter = state->letter;
//	one_letter = (nfa_state_one_letter*)l_calloc(sizeof(nfa_state_one_letter) * DEFAULT_LETTER_COUNT);
//}
//
//void init_one_letter(nfa_state_one_letter *letter)
//{
//	nfa_state** state = letter->next_state;
//	state = (nfa_state**)l_calloc(sizeof(nfa_state*) * DEFAULT_LETTER_NEXT_STATE);
//}
//
//void add_next_state(nfa_state *from_state, nfa_state *to_state, char letter)
//{
//	int i;
//	nfa_state_one_letter *one_letter = from_state->letter;
//	nfa_state* next_state;
//	for (i = 0; i < from_state->letter_num; i++) {
//		if (one_letter[i].letter == letter){
//			one_letter[i].next_state[one_letter[i].state_num] = to_state;
//			one_letter[i].state_num++;
//			break;
//		} else {
//			init_one_letter(one_letter);
//
//			one_letter = (nfa_state_one_letter*)(one_letter + sizeof(nfa_state_one_letter) * from_state->letter_num);
//			from_state->letter_num++;
//
//			one_letter->letter = letter;
//			one_letter->next_state[one_letter->state_num] = to_state;
//			one_letter->state_num++;
//		}
//	}
//}
//
//
//void free_nfa_state(nfa_state* state)
//{
//	int i;
//	nfa_state_one_letter* one_letter = state->letter;
//	for(i = 0; i < state->letter_num; i++) {
//		free_one_letter(one_letter + sizeof(nfa_state_one_letter) * i);
//	}
//
//	if (state->is_acc_stat) {
//		l_free(state->acc_token);
//	}
//	l_free(one_letter);
//}
//
//void free_one_letter(nfa_state_one_letter* letter)
//{
//	l_free(letter->next_state);
//}
//
//void add_accepting_token(nfa_state* state, int token) {
//	state->is_acc_stat =1;
//	state->acc_token = (accept_toekn*)l_calloc(sizeof(accept_toekn));
//	state->acc_token->token = token;
//}

state* create_state(char letter, state* out1, state* out2)
{
	state* state_create;

	state_create = (state*)l_malloc(sizeof(state));

	state_create->letter = letter;
	state_create->out1 = out1;
	state_create->out2 = out2;

	return state_create;
}

void destory_state(state* state_destory)
{
	l_free(state_destory);
}

frage* create_frage(state* start, state* out)
{
	frage* frage_create;

	frage_create = (frage*)l_malloc(sizeof(state));

	frage_create->start = start;
	frage_create->out = out;

	return frage_create;
}

void destroy_frage(frage* frage)
{
	l_free(frage);
}

frage* concate_frage(frage* front_frage, frage* rear_frage)
{
	(front_frage->out)->out1 = rear_frage->start;
	front_frage->out = rear_frage->out;
	destroy_frage(rear_frage);
	return front_frage;
}

frage* union_frage(frage* frage1, frage* frage2)
{
	state *start, *out;

	start = create_state(CLOSURE, frage1->start, frage2->start);
	out = create_state(CLOSURE, frage1->out, frage2->out);

	frage1->start = start;
	frage1->out = out;

	l_free(frage2);

}
