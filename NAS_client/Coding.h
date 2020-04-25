#ifndef CODING_H_
#define CODING_H_
#include <iostream>
#include <string.h>



int ascii_ti_int(char symbol);
void decode_signal(char* src, char* dst, int src_length);
void code_signal(char* src, char* dst, int src_length);
void string_into_array(std::string* note, char* buffer, int buffer_length);

#endif
