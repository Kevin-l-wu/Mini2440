#ifndef STRING_H_
#define STRING_H_

int strcmp(char* dest_str1, char* dest_str2);
int strlen(char* dest_str);
int strcpy(char* dest_str, char* src_str);
int memcpy(char* dest_str, char* src_str, int length);
void clean_string(char* dest_string);
void memset(void* s, int ch, unsigned n);
void clean_string_prox(char* dest_string);
int string_to_int(char* source_string);

#endif
