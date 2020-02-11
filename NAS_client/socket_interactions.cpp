
#include "socket_interactions.h"
int COMMAND_LENGTH = 26;
int ANSWER_LENGTH = 20;

struct Answer* get_answer(int socket)
{
	char buffer[ANSWER_LENGTH];
	char work_buffer[ANSWER_LENGTH * 2];

	std::string command = "";
	struct Answer* current_answer = new struct Answer;
	read_into_buffer(socket, buffer, ANSWER_LENGTH);
	decode_signal(buffer, work_buffer, ANSWER_LENGTH);
	std::cout << "decode:  ";
	for (int i = 0; i < ANSWER_LENGTH * 2; i++)
	{
		std::printf("%c", work_buffer[i]);
	}
	std::cout << std::endl;
	char_array_into_string(work_buffer, &command, ANSWER_LENGTH * 2);
	command_parser(&command, current_answer);
	return current_answer;
}



std::string* get_additional_fields(int socket, int amount)
{
	char additional_buffer[amount];
	char work_additional_buffer[2 * amount];
	read_into_buffer(socket, additional_buffer, amount);
	decode_signal(additional_buffer, work_additional_buffer, amount);
	std::string* additional_fields = new std::string;
	char_array_into_string(work_additional_buffer, additional_fields, 2 * amount );
	//std::cout << " length "  << additional_fields->length();
	return additional_fields;
}

void command_parser(std::string* src, struct Answer* req)
{
	req->header = std::stoi(src->substr(0, 8) , 0, 16);
	req->flags  = std::stoi(src->substr(8, 8), 0, 16);
	req->start  = std::stoi(src->substr(16, 8), 0, 16);
	req->cnt    = std::stoi(src->substr(24, 8), 0, 16);
	req->cmd    = std::stoi(src->substr(32, 4), 0, 16);
	req->fmt    = std::stoi(src->substr(36, 4), 0, 16);
}


//header   flags    start    cnt      cmd  fmt
//30000002 00000000 00000000 00000000 0003 0000
std::ostream& operator << (std::ostream& stm, const struct Answer& req)
{
    stm  << "\nheader: " << std::hex << req.header <<
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


	struct Answer* current_answer = get_answer(socket);
	std::cout << *current_answer;
	std::string* additional_fields = get_additional_fields(socket, current_answer->cnt);
	//std::cout << " length "  << additional_fields->length();
	std::cout << "additional was: " << *additional_fields << "\n";
}


std::string* char_array_into_string(char* buffer, std::string* str, int buffer_size)
{
	//std::cout << "size:  "<< buffer_size << std::endl;
	for (int i = 0; i < buffer_size; i++) {
		*str += buffer[i];

	}
	return str;
}
