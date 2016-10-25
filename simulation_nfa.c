#include "lcc.h"
int simulation_nfa(state_list* start_state, char* letter, int length)
{
	int i, j;
	state_list *list_1, *list_2;

	list_1 = start_state;

	for (i = 0; i < length; i++) {
			get_next_list(list_1, list_2, *(letter + i));
			list_1 = list_2;
	}
}

void make_move(state* dest_state, state_list* end_list, char letter)
{
	if (dest_state != NULL) {
		if (dest_state->letter == letter) {
			if (!is_in_state_list(end_list, dest_state)) {
				add_state_to_list(end_list, dest_state);
			}
		}
	}
}

void get_next_list(state_list* start_list, state_list* end_list, char letter)
{
	int i, j;
	state *start, *end;

	for (i = 0; i < start_list->num; i++) {
		start = start_list->state_array[i];
		end = start->out1;
		make_move(end, end_list, letter);
		closure_state(end_list, end);
		end = start->out2;
		make_move(end, end_list, letter);
		closure_state(end_list, end);
	}
}


void closure_state(state_list* list, state* start)
{
	int i, j;
	state *end;
	end = start->out1;
	make_move(end, list, CLOSURE);
	closure_state(list, end);
	end = start->out2;
	make_move(end, list, CLOSURE);
	closure_state(list, end);
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

void add_state_to_list(state_list* list, state* s)
{
	list->state_array[list->num] = s;
	list->num++;
}
