
#include "Requesthandler.h"
extern std::list<struct Box_info> global_boxes;


void Request_handler::handle_request(int socket, int id) {


	struct Request* current_request = get_command(socket);
	if (current_request == nullptr)
	{
		return;
	}
	std::cout << *current_request;


	std::string* answer = handle_comand(current_request, socket);

	//std::cout << "length:  " << answer->length() << "  " << *answer;
	Socket_interactions::write_command(socket, answer);
	close(socket);
}



struct Request* Request_handler::get_command(int socket)
{
	char buffer[COMMAND_LENGTH];
	char work_buffer[COMMAND_LENGTH * 2];
	std::string command = "";
	struct Request* current_requuest = new struct Request;
	bool error = Socket_interactions::read_into_buffer(socket, buffer, COMMAND_LENGTH);
	if (error)
	{
		return nullptr;
	}
	decode_signal(buffer, work_buffer, COMMAND_LENGTH);
	char_array_into_string(work_buffer, &command, COMMAND_LENGTH * 2);
	error = command_parser(&command, current_requuest);
	if (error)
	{
		return nullptr;
	}
	return current_requuest;
}



// device header   flags    start    cnt      cmd  fmt   mhop
// 0001   30000002 00000000 00000000 00000000 0003 0000  00000001
//
bool Request_handler::command_parser(std::string* src, struct Request* req)
{
	try{
		req->device = std::stoi(src->substr(0, 4) , 0, 16);
		req->header = std::stoi(src->substr(4, 8) , 0, 16);
		req->flags  = std::stoi(src->substr(12, 8), 0, 16);
		req->start  = std::stoi(src->substr(20, 8), 0, 16);
		req->cnt    = std::stoi(src->substr(28, 8), 0, 16);
		req->cmd    = std::stoi(src->substr(36, 4), 0, 16);
		req->fmt    = std::stoi(src->substr(40, 4), 0, 16);
		req->mhop   = std::stoi(src->substr(44, 8), 0, 16);
	}
	catch (std::exception& e)
	{
	    std::cout << "Standard exception: " << e.what() << std::endl;
	    return true;
	}
	return false;
}

Box* Request_handler::find_box_by_name(int name)
{
	for(auto& box_info : global_boxes)
	{
		if (box_info.box_name == name)
		{
			return box_info.box;
		}
	}
	return nullptr;
}

Box* Request_handler::find_box_by_device(int device)
{
	for(auto& box_info : global_boxes)
	{
		for (auto& diskinfo : *(box_info.box->get_disks()))
		{
			if (diskinfo.disk->get_number() == device)
			{
				return box_info.box;
			}
		}
	}
	return nullptr;
}


std::string* Request_handler::handle_comand(struct Request* request, int socket) {
//	std::string* additional_fields = get_additional_fields(socket, current_request->cnt);
//	std::cout << "additional was: " << *additional_fields << " from " << id << "\n";
	std::string* result = new std::string;
	std::string addit = "";
	Answer* answer = new Answer;
	Box* current_box;
	switch (request->cmd)
	{
		case 1:
			current_box = find_box_by_name(request->mhop);
			if (current_box == nullptr)
			{
				*result = "2280000000000000000000000000000000000000000000000228";
				return result;
			}
			addit = current_box->make_answer_is_device_in_box(request->device, answer);
			break;
		case 4:
			current_box = find_box_by_device(request->device);
			if (current_box == nullptr)
			{
				std::cout << "Box with device " << request->device << " not found" << std::endl;
				*result = "2280000000000000000000000000000000000000000000000228";
				return result;
			}
			addit = current_box->find_all_coping(request, answer);
			break;
		case 5:
			current_box = find_box_by_device(request->device);
			if (current_box == nullptr)
			{
				std::cout << "Box with device " << request->device << " not found" << std::endl;
				*result = "2280000000000000000000000000000000000000000000000228";
				return result;
			}
			current_box->make_local_coping(socket, request, answer);
			break;
	}


	*result = *answer_to_string(answer) + addit + "\0";
	std::cout << "answer to client:  " <<*result << std::endl;
	return result;
}
