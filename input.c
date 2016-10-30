#include "lcc.h"

void loadfile(char* file_name, input_file* file)
{
	HANDLE handle_file, handle_file_mapping;
	handle_file = CreateFile(file_name, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	file->h_file = handle_file;

	file->size = GetFileSize(handle_file, NULL);

	handle_file_mapping = CreateFileMapping(handle_file, NULL, PAGE_READWRITE, 0, file->size + 1, NULL);
	if ( handle_file_mapping == NULL) {
		log_error("create file mapping fail\n");
	}
	file->h_filemapping = handle_file_mapping;

	file->cursor = file->base = (unsigned char*)MapViewOfFile(handle_file_mapping, FILE_MAP_WRITE, 0, 0, 0);
	if (file->cursor == NULL) {
		log_error("map file fail\n");
	}

	file->file_name = file_name;
	file->line = 1;
	file->base[file->size] = END_OF_FILE;
}

void unloadfile(input_file* file)
{
	UnmapViewOfFile(file->base);
	CloseHandle(file->h_filemapping);
	SetFilePointer(file->h_file, file->size, NULL, FILE_BEGIN);
	SetEndOfFile(file->h_file);
	CloseHandle(file->h_file);
}

unsigned char* get_next_line(input_file* file)
{
	while (*(file->cursor) != '\n' && *(file->cursor) != END_OF_FILE) {
		file->cursor++;
	}

	if (*(file->cursor) == END_OF_FILE) {
		return NULL;
	}

	file->line++;
	file->cursor++;
	return file->cursor;
}

unsigned char* get_next_char(input_file* file)
{
	file->cursor++;
	return file->cursor;
}

void main()
{
	input_file file;
	state_list* start_list;

	init_logfile_fd(5);
	loadfile("C:\\lex.txt", &file);
	start_list = parse_regular_expression(&file);
	simulation_nfa(start_list, "higk", 4);

}

