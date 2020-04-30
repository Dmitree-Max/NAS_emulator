
#ifndef MAIN_FUNCTIONS_H_
#define MAIN_FUNCTIONS_H_


#include <iostream>
#include <string>
#include <list>
#include "../NAS_client/NAS_client.h"



extern std::list<struct hash_table_node*> DSS;
struct first_command_answer
{
	int box;
	int device;
	int sym;
	int number;
	int box_version;
};

void get_box_and_disks_configuration();
bool parse_answer(std::string* src, struct Answer* ans);
std::string make_simple_command(int device, int cnt, int cmd);
bool parse_first_command_additional_fields(std::string* src, struct first_command_answer* answer);
bool parse_second_command_additional_fields(std::string* src, std::list<int>* answer);
struct hash_table_node* find_box(int number);
std::string to_hex(int number);
void get_other_disks();
void print_structure();



struct hash_table_node
{
	int box_number;
	std::list<int >* disks;
	std::list<std::pair<int, int> >* gatekeepers;
};
#endif
