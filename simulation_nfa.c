#include "lcc.h"

int list_id = 0;

void add_state_to_list(state_list* list, state* s)
{
	list->state_array[list->num] = s;
	list->num++;
	s->list_id = list_id;
}

BOOL is_in_state_list(state_list* list, state* s)
{
	int i;
	int len = list->num;

	for (i = 0; i < len; i++) {
		if (list->state_array[i]->list_id == s->list_id) {
			return TRUE;
		}
	}
	return FALSE;
}

BOOL can_make_move(state* state, char letter)
{
	if(state->letter == letter) {
		return TRUE;
	}
	return FALSE;
}

void add_out_and_closure_to_list(state_list* list, state* start)
{
	state* end;

	end = start->out1;
	if (end != NULL) {
		if (!is_in_state_list(list, end)) {
			add_state_to_list(list, end);
			closure_state(list, end);
		}
	}
	end = start->out2;
	if (end != NULL) {
		if (end != NULL) {
			if (!is_in_state_list(list, end)) {
				add_state_to_list(list, end);
				closure_state(list, end);
			}
		}
	}
}

void get_next_list(state_list* start_list, state_list* end_list, char letter)
{
	int i, j;
	state *start, *end;

	list_id++;		//mark state_list

	for (i = 0; i < start_list->num; i++) {
		start = start_list->state_array[i];

		if (can_make_move(start, letter)) {
			add_out_and_closure_to_list(end_list, start);
		}
	}
}

void closure_state(state_list* list, state* start)
{
	int i, j;

	if (can_make_move(start, CLOSURE)) {
		add_out_and_closure_to_list(list, start);
	}
}

void closure_list(state_list* list_in)
{
	int i;

	for (i = 0; i < list_in->num; i++) {
		closure_state(list_in, list_in->state_array[i]);
	}
}

void swap_list_addr(state_list** list_1, state_list** list2)
{
	state_list* list_temp;
	memset(*list_1, 0, sizeof(state_list));
	list_temp = *list_1;
	*list_1 = *list2;
	*list2 = list_temp;
}

int simulation_nfa(state_list* start_list, char* letter, int length)
{
	int i, j;
	state_list *list_1, *list_2;
	state** state_array;
	int token = 0;

	list_1 = (state_list *)calloc(1, sizeof(state_list));
	list_2 = (state_list *)calloc(1, sizeof(state_list));

	l_memcpy(list_1, start_list, sizeof(state_list));
	closure_list(list_1);

	for (i = 0; i < length; i++) {
		get_next_list(list_1, list_2, *(letter + i));
		swap_list_addr(&list_1, &list_2); 
	}
	state_array = list_1->state_array;
	for (i = 0; i < list_1->num; i++) {
		if(state_array[i]->letter = ACCEPTING) {
			token = ((accepting_state*)state_array[i])->token;
			log_debug("token is :%d\n", token);
		}
	}

	l_free(list_1);
	l_free(list_2);

	return token;
}
