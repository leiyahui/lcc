#ifndef _INPUT_H
#define _INPUT_H

#define  END_OF_FILE  255

typedef struct _input_file {
	char* file_name;
	unsigned char* base;
	unsigned char *cursor;
	HANDLE h_file;
	HANDLE h_filemapping;
	int line;
	int size;
}input_file;

void loadfile(char* file_name, input_file* file);
void unloadfile(input_file* file);

unsigned char* get_next_line(input_file* file);
unsigned char* get_next_char(input_file* file);

#endif