#include "main_functions.h"


void get_box_and_disks_configuration()
{
	for (int i = 1; i < 1000; i++)
	{
		std::string command = make_simple_command(i, 0, 1);
		//std::cout << command;
		std::string answer = nasclient::send_command(command, "127.0.0.1", 5060);
		//std::cout << answer;
		struct Answer* main_answer = new struct Answer;
		if (parse_answer(&answer, main_answer))
		{
			std::string addit = answer.substr(40, 40);
			struct first_command_answer* disk_info = new struct first_command_answer;
			if (main_answer->cnt == 1)
			{
				if(parse_first_command_additional_fields(&addit, disk_info))
				{
					struct hash_table_node* box = find_box(disk_info->box);
					if (box == nullptr)
					{
						//std::cout << "here";
						struct hash_table_node* new_box = new struct hash_table_node;
						std::list<std::pair<int, int> >* new_gatekeepers = new std::list<std::pair<int, int> >;
						std::list<int>* new_disks = new std::list<int>;
						new_box->box_number = disk_info->box;
						new_box->gatekeepers = new_gatekeepers;
						new_box->disks = new_disks;
						new_gatekeepers->push_back(std::make_pair(disk_info->device, disk_info->sym));
						structure->get_DSS()->push_back(new_box);
					}
					else
					{
						box->gatekeepers->push_back(std::make_pair(disk_info->device, disk_info->sym));
					}
				}
				else
				{
					std::cout << "additional answer parse error" << std::endl;
				}
			}
			delete(disk_info);
		}
		else
		{
			std::cout << "main answer parse error" << std::endl;
			break;
		}
		delete(main_answer);
	}
}


// device header   flags    start    cnt      cmd  fmt   mhop
// 0001   30000002 00000000 00000000 00000000 0003 0000  00000001
//
bool parse_request(std::string* src, struct Request* req)
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
	    return false;
	}
	return true;
}


int is_request_valuable(std::string request, std::list<std::string>* additional_requests)
{
	std::string requst_string = request.substr(0, 54);
	std::string additional = request.substr(54, request.length() - 54);
	struct Request* req = new struct Request;
	if (parse_request(&requst_string, req))
	{
		switch (req->cmd)
		{
			case 3:
				return make_local_pairs(req, &additional);
			case 5:
				return activate_local_pairs(req, &additional);
			case 7:
				return remove_local_pairs(req, &additional);
			case 10:
				return make_remote_pairs(req, &additional);
			case 11:
				return remove_remote_pairs(req, &additional);
			case 9:
				return make_groups(req, &additional);
			case 12:
				return delete_groups(req, &additional);
			default:
				delete(req);
				return -2;
		}
	}
	else
	{
		delete(req);
		return -1;
	}
}


int make_groups(struct Request* req, std::string* additional)
{
	int displacement = 0;
	for (int i = 0; i < req->cnt; i++)
	{
		int box = structure->find_box_by_device(req->device);
		int src_box = std::stoi(additional->substr(displacement, 8), 0, 16);
		if (box != src_box)
		{
			return 1;
		}
		int src_group = std::stoi(additional->substr(displacement +8, 8), 0, 16);
		int dst_box = std::stoi(additional->substr(displacement + 16, 8), 0, 16);
		int result = can_make_group(src_box, dst_box, src_group);
		if (result != 0)
			return result;
		displacement += 32;
	}
	return 0;
}


int delete_groups(struct Request* req, std::string* additional)
{
	int displacement = 0;
	for (int i = 0; i < req->cnt; i++)
	{
		int group = std::stoi(additional->substr(displacement, 8), 0, 16);
		int result = can_delete_group(group);
		if (result != 0)
			return result;
		displacement += 8;
	}
	return 0;
}


int can_make_group(int src_box, int dst_box, int src_group)
{
	if (!structure->is_there_such_box(dst_box))
	{
		return 2;
	}
	if (structure->calc_amount_groups_with_box(src_box) >= 255)
		return 4;
	if (structure->calc_amount_groups_with_box(dst_box) >= 255)
		return 4;
	structure->make_group(src_group, src_box, dst_box);
	return 0;
}


int can_delete_group(int group)
{
	int delete_status = structure->delete_group(group);

	// no such group
	if (delete_status == -1)
		return 2;

	//there is active coping in the group
	if (delete_status == -2)
		return 1;
	return 0;
}

int activate_local_pairs(struct Request* req, std::string* additional)
{
	int displacement = 0;
	for (int i = 0; i < req->cnt; i++)
	{
		int box = structure->find_box_by_device(req->device);
		int src_sym = std::stoi(additional->substr(displacement, 8), 0, 16);
		int dst_sym = std::stoi(additional->substr(displacement +8, 8), 0, 16);
		int result = can_activate_local_pair(box, src_sym, dst_sym);
		if (result != 0)
			return result;
		displacement += 16;
	}
	return 0;
}


int remove_local_pairs(struct Request* req, std::string* additional)
{
	int displacement = 0;
	for (int i = 0; i < req->cnt; i++)
	{
		int box = structure->find_box_by_device(req->device);
		int dst_sym = std::stoi(additional->substr(displacement, 8), 0, 16);
		int result = can_remove_local_pair(box, dst_sym);
		if (result != 0)
			return result;
		displacement += 8;
	}
	return 0;
}


int make_local_pairs(struct Request* req, std::string* additional)
{
	int displacement = 0;
	for (int i = 0; i < req->cnt; i++)
	{
		int box = structure->find_box_by_device(req->device);
		int src_sym = std::stoi(additional->substr(displacement, 8), 0, 16);
		int dst_sym = std::stoi(additional->substr(displacement + 8, 8), 0, 16);
		int result = can_make_local_pair(box, src_sym, dst_sym);
		if (result != 0)
			return result;
		displacement += 16;
	}
	return 0;
}


int make_remote_pairs(struct Request* req, std::string* additional)
{
	int displacement = 0;
	for (int i = 0; i < req->cnt; i++)
	{
		int src_box = structure->find_box_by_device(req->device);
		int group 	= std::stoi(additional->substr(displacement    ,  8), 0, 16);
		int src_sym = std::stoi(additional->substr(displacement + 8,  8), 0, 16);
		int dst_sym = std::stoi(additional->substr(displacement + 16, 8), 0, 16);
		int dst_box = structure->find_box_by_group(src_box, group);
		int result  = can_make_remote_pair(group, src_box, dst_box, src_sym, dst_sym);
		if (result != 0)
			return result;
		displacement += 24;
	}
	return 0;
}


int remove_remote_pairs(struct Request* req, std::string* additional)
{
	int displacement = 0;
	for (int i = 0; i < req->cnt; i++)
	{
		int src_box = structure->find_box_by_device(req->device);
		int group 	= std::stoi(additional->substr(displacement    ,  8), 0, 16);
		int src_sym = std::stoi(additional->substr(displacement + 8,  8), 0, 16);
		int dst_sym = std::stoi(additional->substr(displacement + 16, 8), 0, 16);
		int dst_box = structure->find_box_by_group(src_box, group);
		if (dst_box == 0)
			return 4;
		int result  = can_remove_remote_pair(group, src_box, dst_box, src_sym, dst_sym);
		if (result != 0)
			return result;
		displacement += 24;
	}
	return 0;
}


int can_make_local_pair(int box, int src_sym, int dst_sym)
{
	int dst_status = structure->is_it_dist(box, dst_sym);
	if (dst_status >= 1)
	{
		int src_status = structure->is_it_src_to(box, 0, src_sym, dst_sym);
		if ((src_status == 1) or (src_status == 4))
			return 1;
		if (src_status == -1)
			return 2;
		return 4;
	}
	if (dst_status == -1)
		return 2;

	structure->make_pair(box, box, 0, src_sym, dst_sym);
	return 0;
}


int can_remove_remote_pair(int group, int src_box, int dst_box, int src_sym, int dst_sym)
{
	int dst_status = structure->is_it_dist(dst_box, dst_sym);
	if (dst_status >= 1)
	{
		int src_status = structure->is_there_remote_pair(group, src_sym, dst_sym);
		if (src_status == 4)
			return 1;
		if (src_status == -1)
			return 2;
		if (src_status == 0)
			return 5;
		if (src_status == 1)
		{
			structure->remove_remote_pair(src_box, dst_box, group, src_sym, dst_sym);
			return 0;
		}
	}
	if (dst_status == -1)
		return 2;
	return 5;
}


int can_make_remote_pair(int group, int src_box, int dst_box, int src_sym, int dst_sym)
{
	int dst_status = structure->is_it_dist(dst_box, dst_sym);
	int src_status = structure->is_it_dist(src_box, src_sym);
	if (dst_status >= 1 or src_status >= 1)
	{
		int src_status = structure->is_there_remote_pair(group, src_sym, dst_sym);
		if ((src_status == 1) or (src_status == 4))
			return 1;
		if (src_status == -1)
			return 2;
		return 4;
	}
	if (dst_status == -1 or src_status == -1)
		return 2;
	structure->make_remote_pair(src_box, dst_box, group, src_sym, dst_sym);
	return 0;
}


int can_remove_local_pair(int box, int dst_sym)
{
	int dst_status = structure->is_it_dist(box, dst_sym);
	int src_sym = 0;
	if (dst_status >= 1)
	{
		src_sym = dst_status; 		//if dst_status >= 1, there is src_sym
		int src_status = structure->is_it_src_to(box, 0, src_sym, dst_sym);
		if (src_status == 4)
			return 1;
		if (src_status == -1)
			return 2;
		if (src_status == 0)
			return 4;
	}
	if (dst_status == -1)
		return 2;

	structure->delete_pair(box, 0, src_sym, dst_sym);
	return 0;
}


int can_activate_local_pair(int box, int src_sym, int dst_sym)
{
	int dst_status = structure->is_it_dist(box, dst_sym);
	if (dst_status >= 1)
	{
		int src_status = structure->is_it_src_to(box, 0, src_sym, dst_sym);
		if (src_status == 0)
			return 1;
		if (src_status == -1)
			return 2;
		if (src_status == 4) //coping is active
			return 4;
		return 0;
	}
	if (dst_status == -1)
		return 2;
	if (dst_status == 0)
		return 1;

	structure->make_pair(box, box, 0, src_sym, dst_sym);
	return 0;
}


void get_other_disks()
{
	for (auto box : *(structure->get_DSS()))
	{
		if(box->gatekeepers->size() == 0)
		{
			std::cout << "no gatekeepers"<<std::endl;
			continue;
		}
		else
		{
			int some_gatekeeper = box->gatekeepers->front().first;
			std::string command = make_simple_command(some_gatekeeper, 10000, 8);
			std::string answer = nasclient::send_command(command, "127.0.0.1", 5060);
			//std::cout << answer;
			struct Answer* main_answer = new struct Answer;
			if (parse_answer(&answer, main_answer))
			{
				std::string addit = answer.substr(40, answer.length() - 40);
				//std::cout << addit << "\n";
				std::list<int> disks_info;
				if (main_answer->cnt != 0)
				{
					if(parse_second_command_additional_fields(&addit, &disks_info))
					{
						for (int disk : disks_info)
						{
							box->disks->push_back(disk);
						}

					}
					else
					{
						std::cout << "additional answer parse error" << std::endl;
					}
				}
			}
			else
			{
				std::cout << "main answer parse error" << std::endl;
			}
			delete(main_answer);
		}
	}
}


std::string to_hex(int number)
{
	std::stringstream stream;
	stream << std::hex << number;
	std::string result( stream.str() );
	result = "00000000" + result;
	return result.substr(result.length() - 8, 8);
}


std::string make_simple_command(int device, int cnt, int command)
{
	std::string result = to_hex(device).substr(4, 4) + "290000000000000000000000" + to_hex(cnt) + to_hex(command).substr(4, 4) + "000000000000";
	return result;
}


struct hash_table_node* find_box(int number)
{
	for (struct hash_table_node* el : (*structure->get_DSS()))
	{
		if (el->box_number == number)
		{
			return el;
		}
	}
	return nullptr;
}


bool parse_first_command_additional_fields(std::string* src, struct first_command_answer* answer)
{
	try{
		answer->box			 = std::stoi(src->substr(0 , 8), 0, 10);
		answer->device  	 = std::stoi(src->substr(8 , 8), 0, 10);
		answer->sym  		 = std::stoi(src->substr(16, 8), 0, 10);
		answer->number       = std::stoi(src->substr(24, 8), 0, 10);
		answer->box_version  = std::stoi(src->substr(32, 8), 0, 10);
	}
	catch (std::exception& e)
	{
	    std::cout << "Standard exception: " << e.what() << std::endl;
	    return false;
	}
	return true;
}


bool parse_second_command_additional_fields(std::string* src, std::list<int>* answer)
{
	int length = src->length();
	//std::cout << length << "\n";
	if (length % 8 != 0)
	{
		std::cout << "length is not x * 8\n";
		return false;
	}
	for (int i = 0; i < length / 8; i++)
	{
		try{
			int number = std::stoi(src->substr(i * 8, 8) , 0, 10);
			answer->push_back(number);
		}
		catch (std::exception& e)
		{
			std::cout << "Standard exception: " << e.what() << std::endl;
			std::cout << src->substr(i * 8, 8) << " subs\n";
			return false;
		}
	}
	return true;
}


bool parse_answer(std::string* src, struct Answer* answer)
{
	try{
		answer->header = std::stoi(src->substr(0, 8) , 0, 10);
		answer->flags  = std::stoi(src->substr(8, 8) , 0, 10);
		answer->start  = std::stoi(src->substr(16, 8), 0, 10);
		answer->cnt    = std::stoi(src->substr(24, 8), 0, 10);
		answer->cmd    = std::stoi(src->substr(32, 4), 0, 10);
		answer->fmt    = std::stoi(src->substr(36, 4), 0, 10);
	}
	catch (std::exception& e)
	{
	    std::cout << "Standard exception: " << e.what() << std::endl;
	    return false;
	}
	return true;
}


void print_structure()
{
	for (struct hash_table_node* node : (*structure->get_DSS()))
	{
		std::cout << "Box " << node->box_number << ": \n  gatekeepers:\n";
		for (auto gk : *(node->gatekeepers))
		{
			std::cout << "      disk: " << gk.first << "  sym:  " << gk.second << "\n";
		}
		if (node->disks->size() == 0)
		{
			continue;
		}
		std::cout << "\n  disks:\n";
		for (auto disk : *(node->disks))
		{
			std::cout << "      sym:  " << disk << "\n";
		}
	}
}
