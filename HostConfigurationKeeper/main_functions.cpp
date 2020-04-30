#include "main_functions.h"

std::list<struct hash_table_node*> DSS;


void get_box_and_disks_configuration()
{
	for (int i = 34; i < 35; i++)
	{
		std::string command = make_simple_command(i, 1);
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
						std::list<std::pair<int, int> >* new_disks = new std::list<std::pair<int, int> >;
						new_box->box_number = disk_info->box;
						new_box->disks = new_disks;
						new_disks->push_back(std::make_pair(disk_info->device, disk_info->sym));
						DSS.push_back(new_box);
					}
					else
					{
						box->disks->push_back(std::make_pair(disk_info->device, disk_info->sym));
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
		}
		delete(main_answer);
	}
}



std::string make_simple_command(int device, int command){
	device += 10000;
	command += 10000;
	std::string result = std::to_string(device).substr(1, 4) + "29000000000000000000000000000000" + std::to_string(command).substr(1, 4)
			+ "000000000000";
	return result;
}


struct hash_table_node* find_box(int number)
{
	for (struct hash_table_node* el : DSS)
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
		answer->box			 = std::stoi(src->substr(0 , 8), 0, 16);
		answer->device  	 = std::stoi(src->substr(8 , 8), 0, 16);
		answer->sym  		 = std::stoi(src->substr(16, 8), 0, 16);
		answer->number       = std::stoi(src->substr(24, 8), 0, 16);
		answer->box_version  = std::stoi(src->substr(32, 8), 0, 16);
	}
	catch (std::exception& e)
	{
	    std::cout << "Standard exception: " << e.what() << std::endl;
	    return false;
	}
	return true;
}


bool parse_answer(std::string* src, struct Answer* answer)
{
	try{
		answer->header = std::stoi(src->substr(0, 8) , 0, 16);
		answer->flags  = std::stoi(src->substr(8, 8) , 0, 16);
		answer->start  = std::stoi(src->substr(16, 8), 0, 16);
		answer->cnt    = std::stoi(src->substr(24, 8), 0, 16);
		answer->cmd    = std::stoi(src->substr(32, 4), 0, 16);
		answer->fmt    = std::stoi(src->substr(36, 4), 0, 16);
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
	for (struct hash_table_node* node : DSS)
	{
		std::cout << "Box " << node->box_number << ": \n  disks: ";
		for (auto disk : *(node->disks))
		{
			std::cout << "      disk: " << disk.first << "  sym:  " << disk.second + "\n";
		}
	}
}
