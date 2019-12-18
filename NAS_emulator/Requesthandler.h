#ifndef REQUESTHANDLER_H_
#define REQUESTHANDLER_H_

#include "Coding.h"
#include "Box.h"
#include <queue>
#include "Net_stuff.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>

struct Box_info {
	Box* box;
	int box_name;
};

// device header   flags    start    cnt      cmd  fmt
// 0001   30000002 00000000 00000000 00000000 0003 0000
struct Request {
	int device;
	int header;
	int comand;
	int flags;
	int start;
	int cnt;
	int cmd;
	int fmt;
};

std::ostream& operator << (std::ostream& stm, const struct Request& req);


class Request_handler {
private:
	Box_info* boxes;
	std::queue<Request> requests;
public:
	Request_handler();
	Box* find_box_by_name(int name);
	virtual ~Request_handler();
	static int make_snapshot(int box, int disk);
	static int get_configuration();
	static int make_link(int box1, int disk1, int box2, int disk2);
	static void handle_request(int socket, int id);
	static char* handle_comand(char* comand, char* buffer, int buffer_size);
	static void read_into_buffer(int socket, char* buffer, int symbol_ammount);
	static void fill_array_with_nules(char* array);
	static void command_parser(std::string* src, struct Request* req);
};






#endif
