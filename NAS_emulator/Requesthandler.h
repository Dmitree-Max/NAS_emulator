#ifndef REQUESTHANDLER_H_
#define REQUESTHANDLER_H_

#include "Coding.h"
#include "Box.h"
#include "interior_formats.h"
#include "Net_stuff.h"

#include <iostream>
#include <queue>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>

struct Box_info {
	Box* box;
	int box_name;
};



class Request_handler {
private:
	std::queue<Request>* requests;
public:
	Request_handler(Box_info* boxes);
	static Box* find_box_by_name(int name);
	~Request_handler();
	static int make_snapshot(int box, int disk);
	static int get_configuration();
	static int make_link(int box1, int disk1, int box2, int disk2);
	static void handle_request(int socket, int id);
	static std::string* get_additional_fields(int socket, int amount);
	static std::string* handle_comand(struct Request* request, std::string* additional_fields);
	static void read_into_buffer(int socket, char* buffer, int symbol_ammount);
	static void fill_array_with_nules(char* array);
	static void command_parser(std::string* src, struct Request* req);
	static struct Request* get_command(int socket);
	static int write_command(int socket, std::string* command);
};






#endif
