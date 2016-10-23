#include "lcc.h"

typedef struct _state_list{
	state* l_state;
	int num;
}state_list;

int simulation_nfa(state* start, char* letter, int length);

void get_start_state(state_list* list_state, state* start_state);

