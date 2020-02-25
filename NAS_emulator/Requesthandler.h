#ifndef REQUESTHANDLER_H_
#define REQUESTHANDLER_H_

#include "Coding.h"
#include "Box.h"
#include "interior_formats.h"
#include "Net_stuff.h"
#include "Socketinteractions.h"


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



namespace Request_handler
{
	Box* find_box_by_name(int name);
	Box* find_box_by_device(int device);
	int make_snapshot(int box, int disk);
	 int get_configuration();
	int make_link(int box1, int disk1, int box2, int disk2);
	void handle_request(int socket, int id);
	struct Request* get_command(int socket);
	std::string* handle_comand(struct Request* request, int socket);
	bool command_parser(std::string* src, struct Request* req);

};






#endif
