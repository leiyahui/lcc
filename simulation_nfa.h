#ifndef _SIMULATION_NFA_H
#define _SIMULATION_NFA_H

#include "lcc.h"

int simulation_nfa(state_list* start_state, char* letter, int length);

void closure_state(state_list* list, state* start);

void add_state_to_list(state_list* list, state* s);
#endif