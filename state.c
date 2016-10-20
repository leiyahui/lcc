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

void in_stack(frage_stack* stack, frage* in_frage_item)
{
	int len = stack->len;

	if (len > FRAGE_STACK_LEN-1){
		log_error("frage stack full\n");
		exit(0);
	}
	stack->frage_list[len].start = in_frage_item->start;
	stack->frage_list[len].out = in_frage_item->out;
	stack->len++;
}

void out_stack(frage_stack* stack, frage* out_frage_item)
{
	int len = stack->len;

	out_frage_item->start = stack->frage_list[len].start;
	out_frage_item->out = stack->frage_list[len].out;
	stack->len--;
}

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

void create_frage(frage_stack* stack, char letter)
{
	int len;
	state* state_new;

	len = stack->len;
	state_new = create_state(letter, NULL, NULL);
	stack->frage_list[len].start = state_new;
	stack->frage_list[len].out = state_new;
}

void concate_frage(frage_stack* stack)
{
	frage front_frage, rear_frage;
	out_stack(stack, &front_frage);
	out_stack(stack, &rear_frage);
	(front_frage.out)->out1 = rear_frage.start;
	front_frage.out = rear_frage.out;

	in_stack(stack, &front_frage);
}

void union_frage(frage_stack* stack)
{
	state *start, *out;
	frage frage1, frage2;

	out_stack(stack, &frage1);
	out_stack(stack, &frage2);

	start = create_state(CLOSURE, frage1.start, frage2.start); 
	out = create_state(CLOSURE, (frage1.out)->out1, (frage2.out)->out1);

	(frage1.out)->out1 = out;
	(frage2.out)->out1 = out;

	frage1.start = start;
	frage1.out = out;

	in_stack(stack, &frage1);
}

void star_frage(frage_stack* stack)
{
	state *start, *out;
	frage frage_star;

	out_stack(stack, &frage_star);

	out = create_state(CLOSURE, (frage_star.out)->out1, NULL);
	start = create_state(CLOSURE, frage_star.start, out);
	(frage_star.out)->out1 = out;
	(frage_star.out)->out2 = start;

	frage_star.start = start;
	frage_star.out = out;

	in_stack(stack, &frage_star);
}
