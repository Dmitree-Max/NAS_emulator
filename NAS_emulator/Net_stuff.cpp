#include "Net_stuff.h"

void string_into_array(std::string* note, char* buffer, int buffer_size) {
	memset(buffer, '0', buffer_size);
	for (int i = 0; i < (*note).length(); i++) {
		buffer[i] = (*note)[i];

	}

}

std::string* char_array_into_string(char* buffer, std::string* str, int buffer_size)
{
	for (int i = 0; i < buffer_size; i++) {
		*str += buffer[i];
	}
	return str;
}








