#include "main_functions.h"

std::list<struct hash_table_node*> DSS;


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
						DSS.push_back(new_box);
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
		}
		delete(main_answer);
	}
}



void get_other_disks()
{
	for (auto box : DSS)
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
	for (struct hash_table_node* node : DSS)
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
