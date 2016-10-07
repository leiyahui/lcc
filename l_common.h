#ifndef _L_COMMON_H
#define _L_COMMON_H

#define BOOL int
#define TRUE 1
#define FALSE 0

void* l_malloc(int size);
void* l_calloc(int size);
void l_free(void* ptr);

unsigned char oa_str_case_equal(const char *str1, const char *str2);
size_t l_strlen(const char *str);
char* l_strcpy(char* dest, const char* src);
char* l_strncpy(char* dest, const char* src, size_t n);

void oa_change_file_size(int fd);

BOOL is_digit(unsigned char character);
BOOL is_letter(unsigned char character);

#endif