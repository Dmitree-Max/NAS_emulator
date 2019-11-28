
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <iostream>
#include <bits/stdc++.h>
#include <string.h>
#define COMMAND_LENGTH 24


void string_into_array(std::string* note, char* buffer, int buffer_size) {
	memset(buffer, '0', sizeof(buffer));

	for (int i = 0; i < buffer_size; i++) {
		if (i < (*note).length()) {
			buffer[i] = (*note)[i];
		} else {
			buffer[i] = '0';
		}
	}

}

std::string* char_array_into_string(char* buffer, std::string* str, int buffer_size)
{
	for (int i = 0; i < buffer_size; i++) {
		*str += buffer[i];
	}
	return str;
}







