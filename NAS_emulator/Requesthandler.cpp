
#include "Requesthandler.h"


std::ostream& operator << (std::ostream& stm, const struct Request& req)
{
    stm << "device: " << req.device << "\nheader: " << req.header <<
    		"\ncomand: " << req.comand << "\nflags: " << req.flags <<
			"\nstart: " << req.start << "\ncnt: " << req.cnt <<
			"\n cmd: " << req.cmd << "\nfmt: " << req.fmt << std::endl;
    return stm;
}


void Request_handler::handle_request(int socket, int id) {
	bool exit = false;
	char buffer[COMMAND_LENGTH];
	char work_buffer[COMMAND_LENGTH * 2];
	int error_code = 0;
	int additional_notes = 0;


	std::string command = "";
	struct Request cur_rec;
	read_into_buffer(socket, buffer, COMMAND_LENGTH);
	printf("%s\n", buffer);
	decode_signal(buffer, work_buffer, COMMAND_LENGTH);
	printf("%s\n", work_buffer);
	char_array_into_string(work_buffer, &command, COMMAND_LENGTH);

	std::cout << command << "\n";
	command_parser(&command, &cur_rec);
	std::cout << cur_rec;
	char additional_buffer[additional_notes * COMMAND_LENGTH];
	char work_additional_buffer[2* additional_notes * COMMAND_LENGTH];
	//read_into_buffer(socket, additional_buffer, additional_notes * COMMAND_LENGTH);
	//decode_signal(additional_buffer, work_additional_buffer);
	std::string ad_command = "";
	//char_array_into_string(work_additional_buffer, &ad_command);


	std::cout << "i got: " << command << " from " << id << "\n";
	std::cout << "additional was: " << ad_command << " from " << id << "\n";
	char ans_buffer[COMMAND_LENGTH];
	write(socket, handle_comand(buffer, ans_buffer, COMMAND_LENGTH), COMMAND_LENGTH);
	close(socket);
}


void Request_handler::read_into_buffer(int socket, char* buffer, int buffer_size)
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


// device header   flags    start    cnt      cmd  fmt
// 0001   30000002 00000000 00000000 00000000 0003 0000
//
void Request_handler::command_parser(std::string* src, struct Request* req)
{
	req->device = std::stoi(src->substr(0, 4), 0, 16);
	req->header = std::stoi(src->substr(4, 8), 0, 16);
	req->flags = std::stoi(src->substr(12, 8), 0, 16);
}


char* Request_handler::handle_comand(char* comand, char* buffer, int buffer_size) {
	std::string answer = "";
	memset(buffer, '0', buffer_size);
	switch (comand[0]) {
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
