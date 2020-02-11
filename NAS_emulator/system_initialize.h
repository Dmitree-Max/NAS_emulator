

#ifndef SYSTEM_INITIALIZE_H_
#define SYSTEM_INITIALIZE_H_

#include "Box.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <map>
#include "Requesthandler.h"


struct About_box{
	int box_name;
	std::list<std::pair<int, int>>* disk_sym;
};

struct Init_obj{
	std::list<int>* disks;
	std::list<struct About_box> boxes;
};

struct Init_obj* parse_file(const char* filename);
int initialize_system(const char* filename);
std::list<std::string>* parse_by_brackets(std::string input);
std::list<std::string>* parse_by_sticks(std::string input);
std::list<int>* find_numbers(std::string input);
std::ostream& operator << (std::ostream& stm, const struct Init_obj& req);
std::ostream& operator << (std::ostream& stm, const struct About_box& req);

#endif
