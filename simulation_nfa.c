#include "lcc.h"

void add_state_to_list(state_list* list, state* s)
{
	list->state_array[list->num] = s;
	list->num++;
}

BOOL is_in_state_list(state_list* list, state* s)
{
	int i;
	int len = list->num;

	for (i = 0; i < len; i++) {
		if (list->state_array[i] == s) {
			return TRUE;
		}
	}
	return FALSE;
}

BOOL make_move(state* dest_state, state_list* end_list, char letter)
{
	if (dest_state != NULL) {
		if (dest_state->letter == letter) {
			if (!is_in_state_list(end_list, dest_state)) {
				add_state_to_list(end_list, dest_state);
				return TRUE;
			}
		}
	}
	return FALSE;
}

void get_next_list(state_list* start_list, state_list* end_list, char letter)
{
	int i, j;
	state *start, *end;

	for (i = 0; i < start_list->num; i++) {
		start = start_list->state_array[i];
		end = start->out1;
		if (make_move(end, end_list, letter)) {
			closure_state(end_list, end);
		}
		end = start->out2;
		if (make_move(end, end_list, letter)) {
			closure_state(end_list, end);
		}
	}
}

void closure_state(state_list* list, state* start)
{
	int i, j;
	state *end;
	end = start->out1;
	if (make_move(end, list, CLOSURE)) {
		closure_state(list, end);
	}
	end = start->out2;
	if (make_move(end, list, CLOSURE)) {
		closure_state(list, end);
	}
}

void closure_list(state_list* list_in, state_list* list_out)
{
	int i;

	for (i = 0; i < list_in->num; i++) {
		closure_state(list_out, list_in->state_array[i]);
	}
}

void swap_list_addr(state_list** list_1, state_list** list2)
{
	state_list* list_temp;
	list_temp = *list_1;
	*list_1 = *list2;
	*list2 = list_temp;
}

int simulation_nfa(state_list* start_list, char* letter, int length)
{
	int i, j;
	state_list *list_1, *list_2;

	list_1 = (state_list *)calloc(1, sizeof(state_list));
	list_2 = (state_list *)calloc(1, sizeof(state_list));

	l_memcpy(list_1, start_list, sizeof(state_list));
	closure_list(list_1, list_2);
	swap_list_addr(&list_1, &list_2);

	for (i = 0; i < length; i++) {
		get_next_list(list_1, list_2, *(letter + i));
		swap_list_addr(&list_1, &list_2);
	}
	for (i = 0; i < list_1->num; i++) {
		if(list_1->state_array[i]->out1 == NULL && list_1->state_array[i]->out2 == NULL) {
			log_debug("get_state");
		}
	}

	l_free(list_1);
	l_free(list_2);
}
