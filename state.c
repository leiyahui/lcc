/*dfa's or nfa's state*/
#include "lcc.h"

void init_frage_stack(frage_stack* stack)
{
	memset(stack, 0, sizeof(frage_stack));
}
void in_frage_stack(frage_stack* stack, frage* in_frage_item)
{
	int len = stack->len;

	if (len > FRAGE_STACK_LEN-1){
		log_error("frage stack full\n");
		exit(0);
	}
	stack->frage_list[len].start = in_frage_item->start;
	stack->frage_list[len].out = in_frage_item->out;
	stack->len++;
}

void out_frage_stack(frage_stack* stack, frage* out_frage_item)
{
	int len;

	(stack->len)--;
	len = stack->len;

	out_frage_item->start = stack->frage_list[len].start;
	out_frage_item->out = stack->frage_list[len].out;
}

state* create_state(char letter, state* out1, state* out2)
{
	state* state_create;

	state_create = (state*)l_malloc(sizeof(state));

	state_create->letter = letter;
	state_create->out1 = out1;
	state_create->out2 = out2;

	return state_create;
}

void destory_state(state* state_destory)
{
	l_free(state_destory);
}

void create_frage(frage_stack* stack, char letter)
{
	int len;
	state* state_new;

	len = stack->len;
	state_new = create_state(letter, NULL, NULL);
	stack->frage_list[len].start = state_new;
	stack->frage_list[len].out = state_new;
	stack->len++;
}

void concate_frage(frage_stack* stack)
{
	frage front_frage, rear_frage;
	out_frage_stack(stack, &front_frage);
	out_frage_stack(stack, &rear_frage);
	(front_frage.out)->out1 = rear_frage.start;
	front_frage.out = rear_frage.out;

	in_frage_stack(stack, &front_frage);
}

void union_frage(frage_stack* stack)
{
	state *start, *out;
	frage frage1, frage2;

	out_frage_stack(stack, &frage1);
	out_frage_stack(stack, &frage2);

	start = create_state(CLOSURE, frage1.start, frage2.start); 
	out = create_state(CLOSURE, (frage1.out)->out1, (frage2.out)->out1);

	(frage1.out)->out1 = out;
	(frage2.out)->out1 = out;

	frage1.start = start;
	frage1.out = out;

	in_frage_stack(stack, &frage1);
}

void star_frage(frage_stack* stack)
{
	state *start, *out;
	frage frage_star;

	out_frage_stack(stack, &frage_star);

	out = create_state(CLOSURE, (frage_star.out)->out1, NULL);
	start = create_state(CLOSURE, frage_star.start, out);
	(frage_star.out)->out1 = out;
	(frage_star.out)->out2 = start;

	frage_star.start = start;
	frage_star.out = out;

	in_frage_stack(stack, &frage_star);
}
