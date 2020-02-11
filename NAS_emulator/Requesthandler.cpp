
#include "Requesthandler.h"
extern std::list<struct Box_info>* global_boxes;

Request_handler::Request_handler(Box_info* input_boxes)
{
	requests = new std::queue<Request>;
}

void Request_handler::handle_request(int socket, int id) {


	struct Request* current_request = get_command(socket);
	std::cout << *current_request;
	std::string* additional_fields = get_additional_fields(socket, current_request->cnt);
	std::cout << "additional was: " << *additional_fields << " from " << id << "\n";

	std::string* answer = handle_comand(current_request, additional_fields);

	//std::cout << "length:  " << answer->length() << "  " << *answer;
	write_command(socket, answer);
	close(socket);
}



struct Request* Request_handler::get_command(int socket)
{
	char buffer[COMMAND_LENGTH];
	char work_buffer[COMMAND_LENGTH * 2];
	std::string command = "";
	struct Request* current_requuest = new struct Request;
	read_into_buffer(socket, buffer, COMMAND_LENGTH);
	decode_signal(buffer, work_buffer, COMMAND_LENGTH);
	char_array_into_string(work_buffer, &command, COMMAND_LENGTH * 2);
	command_parser(&command, current_requuest);
	return current_requuest;
}


int Request_handler::write_command(int socket, std::string* command)
{
	int command_length = (*command).length();
	if (command_length % 2 == 1)
	{
		throw("command length is odd");
	}
	char buffer[command_length];
	char work_buffer[command_length / 2];

	string_into_array(command, buffer, command_length);
	code_signal(buffer, work_buffer, command_length);
	return write(socket, work_buffer, command_length / 2);
}

std::string* Request_handler::get_additional_fields(int socket, int amount)
{
	char additional_buffer[amount];
	char work_additional_buffer[2* amount];
	read_into_buffer(socket, additional_buffer, amount);
	decode_signal(additional_buffer, work_additional_buffer, amount);
	std::string* additional_fields = new std::string;
	char_array_into_string(work_additional_buffer, additional_fields, amount * 2);
	return additional_fields;
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


// device header   flags    start    cnt      cmd  fmt   mhop
// 0001   30000002 00000000 00000000 00000000 0003 0000  00000001
//
void Request_handler::command_parser(std::string* src, struct Request* req)
{
	req->device = std::stoi(src->substr(0, 4) , 0, 16);
	req->header = std::stoi(src->substr(4, 8) , 0, 16);
	req->flags  = std::stoi(src->substr(12, 8), 0, 16);
	req->start  = std::stoi(src->substr(20, 8), 0, 16);
	req->cnt    = std::stoi(src->substr(28, 8), 0, 16);
	req->cmd    = std::stoi(src->substr(36, 4), 0, 16);
	req->fmt    = std::stoi(src->substr(40, 4), 0, 16);
	req->mhop   = std::stoi(src->substr(44, 8), 0, 16);
}

Box* Request_handler::find_box_by_name(int name)
{
	for(auto& box_info : *global_boxes)
	{
		if (box_info.box_name == name)
		{
			return box_info.box;
		}
	}
	return nullptr;
}

std::string* Request_handler::handle_comand(struct Request* request, std::string* additional_fields) {
	std::string* result = new std::string;
	std::string addit;
	Answer* answer = new Answer;
	if (request->cmd == 1)
	{
			Box* current_box = find_box_by_name(request->mhop);
			if (current_box==nullptr)
			{
				*result = "0000000000000000000000000000000000000000000000000228";
				return result;
			}
			addit = current_box->is_device_in_box(request->device, answer);
	}
	*result = *answer_to_string(answer) + addit + "\0";
	return result;
}
