#include "lcc.h"
int simulation_nfa(state_list* start_state, char* letter, int length)
{
	int i, j;
	state_list *list_1, *list_2;

	list_1 = start_state;

	for (i = 0; i < length; i++) {
			move_step(list_1, list_2, *(letter + i));
	}
}

void move_step(state_list* start_list, state_list* end_list, char letter)
{
	int i, j;
	state *start, *end;

	for (i = 0; i < start_list->num; i++) {
		start = start_list->state_array[i];
		if (start->out1 != NULL) {
			end = start->out1;
			if (end->letter == letter) {
				in_start_state_array(end_list, end);
			}
			if (start->out2 != NULL) {
				end = start->out1;
				if (end->letter == letter) {
					in_start_state_array(end_list, end);
				}
			}
		}
	}
}

void closure_state(state_list* list)
{
	int i, j;
	state* s, *out_s;
	for (i = 0; i < list->num; i++) {
		s = list->state_array[i];
		out_s = s->out1;
		if (out_s != NULL) {
			if (out_s->letter == CLOSURE) {
				if (!is_in_state_list(list, out_s)) {
					add_state_to_list(list, out_s);
					closure_state(list, out_s);
				}
			}
			out_s = s->out2;
			if (out_s != NULL) {
				if (out_s->letter == CLOSURE) {
					if (!is_in_state_list(list, out_s)) {
						add_state_to_list(list, out_s);
						closure_state(list, out_s);
					}
				}
			}
		}
	}
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
