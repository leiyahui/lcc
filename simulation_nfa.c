#include "lcc.h"
int simulation_nfa(state_list* start_state, char* letter, int length)
{
	int i, j;
	state_list *list_1, *list_2;

	list_1 = start_state;

	for (i = 0; i < length; i++) {
		for (j = 0; j < start_state->num; j++) {
			
		}
	}
}

void closure_state(state_list* list_in, state_list* list_out)
{
	int i, j;
	for (i = 0; i < list_in->num; i++) {
		if (list_in->state_array[i]->out1 != NULL) {

		}
	}
}



