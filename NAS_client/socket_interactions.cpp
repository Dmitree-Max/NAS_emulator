
#include "socket_interactions.h"
int COMMAND_LENGTH = 26;

struct Answer* get_command(int socket)
{
	char buffer[COMMAND_LENGTH];
	char work_buffer[COMMAND_LENGTH * 2];
	std::string command = "";
	struct Answer* current_answer = new struct Answer;
	read_into_buffer(socket, buffer, COMMAND_LENGTH);
	decode_signal(buffer, work_buffer, COMMAND_LENGTH);
	char_array_into_string(work_buffer, &command, COMMAND_LENGTH * 2);
	command_parser(&command, current_answer);
	return current_answer;
}



std::string* get_additional_fields(int socket, int amount)
{
	char additional_buffer[amount * COMMAND_LENGTH];
	char work_additional_buffer[2* amount * COMMAND_LENGTH];
	read_into_buffer(socket, additional_buffer, amount * COMMAND_LENGTH);
	decode_signal(additional_buffer, work_additional_buffer, amount * COMMAND_LENGTH);
	std::string* additional_fields = new std::string;
	char_array_into_string(work_additional_buffer, additional_fields, amount * COMMAND_LENGTH * 2);
	return additional_fields;
}

void command_parser(std::string* src, struct Answer* req)
{
	req->device = std::stoi(src->substr(0, 4) , 0, 16);
	req->header = std::stoi(src->substr(4, 8) , 0, 16);
	req->flags  = std::stoi(src->substr(12, 8), 0, 16);
	req->start  = std::stoi(src->substr(20, 8), 0, 16);
	req->cnt    = std::stoi(src->substr(28, 8), 0, 16);
	req->cmd    = std::stoi(src->substr(36, 4), 0, 16);
	req->fmt    = std::stoi(src->substr(40, 4), 0, 16);
}


// device header   flags    start    cnt      cmd  fmt
// 0001   30000002 00000000 00000000 00000000 0003 0000
std::ostream& operator << (std::ostream& stm, const struct Answer& req)
{
    stm <<  "device: "  << std::hex <<  req.device << "\nheader: " << std::hex << req.header <<
    	    "\nflags: " << std::hex <<  req.flags  <<
			"\nstart: " << std::hex <<  req.start  << "\ncnt: "    << std::hex << req.cnt    <<
			"\ncmd: "   << std::hex <<  req.cmd    << "\nfmt: "    << std::hex << req.fmt    <<  std::endl;
    return stm;
}


void read_into_buffer(int socket, char* buffer, int buffer_size)
{
	memset(buffer, '0', buffer_size);
	int error_code = 0;
	if ((error_code = read(socket, buffer, buffer_size)) > 0) {
	} else {
		if (error_code < 0) {
			printf("\n Read error \n");
		}
	}
}



void handle_answer(int socket) {


	struct Answer* current_answer = get_command(socket);
	std::cout << *current_answer;
	std::string* additional_fields = get_additional_fields(socket, current_answer->cnt);
	std::cout << "additional was: " << *additional_fields << "\n";
}


std::string* char_array_into_string(char* buffer, std::string* str, int buffer_size)
{
	for (int i = 0; i < buffer_size; i++) {
		*str += buffer[i];
	}
	return str;
}
