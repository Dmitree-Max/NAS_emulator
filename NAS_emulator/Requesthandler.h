#ifndef REQUESTHANDLER_H_
#define REQUESTHANDLER_H_

#include "Coding.h"
#include "Box.h"
#include <queue>
#include "Net_stuff.h"

struct Box_info {
	Box* box;
	int box_name;
};

struct Request {
	int comand;
};

class Request_handler {
private:
	Box_info* boxes;
	std::queue<Request> requests;
public:
	Request_handler();
	Box* find_box_by_name(int name);
	virtual ~Request_handler();
	int make_snapshot(int box, int disk);
	int get_configuration();
	int make_link(int box1, int disk1, int box2, int disk2);
};

char* comand_handler(char* comand, char* buffer);
void read_into_buffer(int socket, char* buffer, int symbol_ammount);

void fill_array_with_nules(char* array)
{
	memset(array, '0', sizeof(array));
}



void handle_request(int socket, int id) {
	bool exit = false;
	char buffer[COMMAND_LENGTH];
	char work_buffer[COMMAND_LENGTH * 2];
	int error_code = 0;
	int additional_notes = 0;


	std::string command = "";
	read_into_buffer(socket, buffer, COMMAND_LENGTH);
	decode_signal(buffer, work_buffer, COMMAND_LENGTH * 2);
	char_array_into_string(work_buffer, &command, COMMAND_LENGTH);
	//additional_notes = command.substr(12, 4);
	std::cout << "substr = " << command.substr(11, 4) << "\n";
	std::cout << "cnt = " << additional_notes << "\n";
	char additional_buffer[additional_notes * COMMAND_LENGTH];
	char work_additional_buffer[2* additional_notes * COMMAND_LENGTH];
	//read_into_buffer(socket, additional_buffer, additional_notes * COMMAND_LENGTH);
	//decode_signal(additional_buffer, work_additional_buffer);
	std::string ad_command = "";
	//char_array_into_string(work_additional_buffer, &ad_command);


	std::cout << "i got: " << command << " from " << id << "\n";
	std::cout << "additional was: " << ad_command << " from " << id << "\n";
	char ans_buffer[COMMAND_LENGTH];
	write(socket, comand_handler(buffer, ans_buffer), COMMAND_LENGTH);
	close(socket);
}

char* comand_handler(char* command, char* buffer) {
	std::string answer = "";
	memset(buffer, '0', sizeof(buffer));
	switch (command[0]) {
	case '1':
		answer = "configur";
		break;
	default:
		answer = "nosuchco";
		break;
	}
	string_into_array(&answer, buffer, COMMAND_LENGTH);
	return buffer;
}




void read_into_buffer(int socket, char* buffer, int symbol_ammount)
{
	fill_array_with_nules(buffer);
	int error_code = 0;
	if ((error_code = read(socket, buffer, symbol_ammount)) > 0) {
	} else {
		if (error_code < 0) {
			printf("\n Read error \n");
		}
	}
}

#endif
