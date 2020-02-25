#include <iostream>
#include <queue>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <exception>

#include "Coding.h"
#include "Box.h"
#include "interior_formats.h"
#include "Net_stuff.h"

#ifndef SOCKETINTERACTIONS_H_
#define SOCKETINTERACTIONS_H_

class Socket_interactions {
public:
	Socket_interactions();
	virtual ~Socket_interactions();
	static std::string* get_additional_fields(int socket, int amount);
	static bool read_into_buffer(int socket, char* buffer, int symbol_ammount);
	static void fill_array_with_nules(char* array);
	static int write_command(int socket, std::string* command);
};

#endif /* SOCKETINTERACTIONS_H_ */
