#ifndef _STATE_H
#define _STATE_H

#define CLOSURE			254
#define ACCEPTING		255
#define FRAGE_STACK_LEN 10
#define	START_STATE_NUM	30


typedef struct _state {
	char letter;
	int list_id;		//used in simulation nfa
	struct _state* out1;
	struct _state* out2;
}state;

typedef struct _accepting_state {
	state accpeting_state;
	int token;
}accepting_state;

typedef struct _frage {		//one frag contains servel state, it's operand of opertor
	state* start;
	state* out;
}frage;

typedef struct _frage_stack {
	frage frage_list[FRAGE_STACK_LEN];
	int len;
}frage_stack;

typedef struct _state_list {
	state* state_array[START_STATE_NUM];
	int num;
}state_list;

void init_frage_stack(frage_stack* stack);
void in_frage_stack(frage_stack* stack, frage* in_frage_item);
void out_frage_stack(frage_stack* stack, frage* out_frage_item);

state* create_state(char letter, state* out1, state* out2);
accepting_state* create_accepting_state(char token);
void destory_state(state* state_destroy);

void create_frage(frage_stack* stack, char letter);
void concate_frage(frage_stack* stack);
void union_frage(frage_stack* stack);
void star_frage(frage_stack* stack);

#endif

