#ifndef _STATE_H
#define _STATE_H

//#define DEFAULT_LETTER_NEXT_STATE	5
//#define DEFAULT_LETTER_COUNT		5

//typedef struct _accept_token {
//	int token;
//}accept_toekn;
//
//typedef struct _nfa_state_one_letter {		//the same letter may have more than one destination state
//	char letter;
//	int state_num;
//	struct _nfa_state** next_state;
//}nfa_state_one_letter;
//
//typedef struct _nfa_state {			//one state's next letter may be various
//	int letter_num;
//	int is_acc_stat;				//is accepting state
//	accept_toekn* acc_token;		//accepting state have token
//	nfa_state_one_letter* letter;
//}nfa_state;
//
//void init_nfa_state(nfa_state* state);
//
//void init_one_letter(nfa_state_one_letter *letter);
//
//void add_next_state(nfa_state *from_state, nfa_state *to_state, char letter);
//
//void free_nfa_state(nfa_state* state);
//
//void free_one_letter(nfa_state_one_letter* letter);
//
//void add_accepting_token(nfa_state* state, int token);

#define CLOSURE 256
#define FRAGE_STACK_LEN 10

typedef struct _state {
	char letter;
	struct _state* out1;
	struct _state* out2;
}state;

typedef struct _frage {		//one frag contains servel state, it's operand of opertor
	state* start;
	state* out;
}frage;

typedef struct _frage_stack {
	frage frage_list[FRAGE_STACK_LEN];
	int len;
}frage_stack;

void init_frage_stack(frage_stack* stack);
void in_frage_stack(frage_stack* stack, frage* in_frage_item);
void out_frage_stack(frage_stack* stack, frage* out_frage_item);
	
state* create_state(char* letter, state* out1, state* out2);
void destory_state(state* state_destroy);

void create_frage(frage_stack* stack, char letter);
void concate_frage(frage_stack* stack);
void union_frage(frage_stack* stack);
void star_frage(frage_stack* stack);


#endif

