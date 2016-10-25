#include "lcc.h"
void* l_malloc(int size){
	void *ptr;
	if (size <= 0) {
		return 0;
	}
	ptr = malloc(size);
	if (ptr == NULL) {
		log_error("malloc memory exhausted!\n");
		exit(0);
	}
}

void* l_calloc(int count, int size) {
	void *ptr;
	if (size <= 0) {
		return NULL;
	}
	ptr = calloc(count, size);
	if(ptr == NULL) {
		log_error("calloc memory exhausted!\n");
		exit(0);
	}
}

void l_memcpy(void* dest_addr, void* src_addr, int len)
{
	if (src_addr != NULL && dest_addr != NULL && len >= 0) {
		memcpy(dest_addr, src_addr, len);
	} else {
		log_error("memcpy error\n");
		exit(0);
	}
}

void l_free(void* ptr) {
	if (ptr != NULL) {
		free(ptr);
	}
}

unsigned char oa_str_case_equal(const char *str1, const char *str2) {
	if (str1 == NULL || str2 == NULL) {
		return 0;
	}
	if (_stricmp(str1, str2) == 0) {
		return 1;
	}
	return 0;
}

size_t l_strlen(const char *str)
{
	size_t result;
	if (str && *str) {
		result = strlen(str);
	} else {
		result = 0;
	}
	return result;
}

char* l_strcpy(char* dest, const char* src)
{
	if (dest == NULL || src == NULL) {
		return NULL;
	}
	return strcpy(dest, src);
}

char* l_strncpy(char* dest, const char* src, size_t n)
{
	if (dest == NULL || src == NULL || n <= 0) {
		return NULL;
	}
	return strncpy(dest, src, n);
}

void oa_change_file_size(int fd) {
#ifndef _WIN32
	ftruncate(fd, 0);
	lseek(fd, 0, SEEK_SET);
#else
	_chsize_s(fd, 0);
	_lseek(fd, 0, SEEK_SET);
#endif
}

BOOL is_digit(unsigned char character)
{
	if (character >= 48 && character <= 57) {
		return TRUE;
	}
	return FALSE;
}
BOOL is_letter(unsigned char character)
{
	if (character >= 65 && character <= 90 || character >= 97 && character <= 122) {
		return TRUE;
	}
	return FALSE;
}