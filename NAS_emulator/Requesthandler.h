#ifndef REQUESTHANDLER_H_
#define REQUESTHANDLER_H_

#include "Coding.h"
#include "Box.h"
#include "interior_formats.h"
#include "Net_stuff.h"
#include "Socterinteractions.h"

#include <iostream>
#include <queue>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <exception>


struct Box_info {
	Box* box;
	int box_name;
	Box_info(){
		box = nullptr;
		box_name = 0;
	}
};



class Request_handler {
private:
	std::queue<Request>* requests;
public:
	Request_handler(Box_info* boxes);
	static Box* find_box_by_name(int name);
	static Box* find_box_by_device(int device);
	~Request_handler();
	static int make_snapshot(int box, int disk);
	static int get_configuration();
	static int make_link(int box1, int disk1, int box2, int disk2);
	static void handle_request(int socket, int id);

	static std::string* handle_comand(struct Request* request, int socket);

	static bool command_parser(std::string* src, struct Request* req);

};






#endif
