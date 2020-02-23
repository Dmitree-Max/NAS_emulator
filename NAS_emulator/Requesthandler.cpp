
#include "Requesthandler.h"
extern std::list<struct Box_info>* global_boxes;

Request_handler::Request_handler(Box_info* input_boxes)
{
	requests = new std::queue<Request>;
}

void Request_handler::handle_request(int socket, int id) {


	struct Request* current_request = Socket_interactions::get_command(socket);
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
	for(auto& box_info : *global_boxes)
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
	for(auto& box_info : *global_boxes)
	{
		for (auto& diskindo : *(box_info.box->get_disks()))
		{
			if (diskindo.sym == device)
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
				*result = "0000000000000000000000000000000000000000000000000228";
				return result;
			}
			addit = current_box->make_answer_is_device_in_box(request->device, answer);
			break;
		case 5:
			current_box = find_box_by_device(request->device);
			if (current_box == nullptr)
			{
				*result = "0000000000000000000000000000000000000000000000000228";
				return result;
			}
			current_box->make_local_coping(socket, request, answer);
			break;
	}


	*result = *answer_to_string(answer) + addit + "\0";
	return result;
}
