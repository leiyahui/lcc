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


typedef struct _state {
	char letter;
	struct _state* out1;
	struct _state* out2;
}state;

typedef struct _frage {		//one frag contains servel state, it's operand of opertor
	state* start;
	state* out;
}frage;


state* create_state(char* letter, state* out1, state* out2);
void destory_state(state* state_destroy);

frage* create_frage(state* start, state* out);
void destroy_frage(frage* start);

frage* concate_frage(frage* front_frage, frage* rear_frage);
frage* union_frage(frage* frage1, frage* frage2);
frage* star_frage(frage* frage_star);


#endif

