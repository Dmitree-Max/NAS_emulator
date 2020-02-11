

#ifndef SOCKET_INTERACTIONS_H_
#define SOCKET_INTERACTIONS_H_

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

#include "Coding.h"

// device header   flags    start    cnt      cmd  fmt
// 0001   30000002 00000000 00000000 00000000 0003 0000
struct Answer {
	int header;
	int flags;
	int start;
	int cnt;
	int cmd;
	int fmt;
};

std::ostream& operator << (std::ostream& stm, const struct Answer& req);
void command_parser(std::string* src, struct Answer* req);
std::string* get_additional_fields(int socket, int amount);
struct Answer* get_answer(int socket);
void read_into_buffer(int socket, char* buffer, int buffer_size);
void handle_answer(int socket);
std::string* char_array_into_string(char* buffer, std::string* str, int buffer_size);


#endif
