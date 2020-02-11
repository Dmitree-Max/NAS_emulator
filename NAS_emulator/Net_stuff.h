
#ifndef NET_STUFF_H_
#define NET_STUFF_H_

#include <string.h>
#include <string>
#define COMMAND_LENGTH 26


void string_into_array(std::string* note, char* buffer, int buffer_size);
std::string* char_array_into_string(char* buffer, std::string* str, int buffer_size);


#endif
