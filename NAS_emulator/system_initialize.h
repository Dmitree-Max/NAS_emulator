

#ifndef SYSTEM_INITIALIZE_H_
#define SYSTEM_INITIALIZE_H_

#include "Box.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <map>
#include "Requesthandler.h"


void initialize_system(const char* filename);
std::list<std::string>* parse_by_brackets(std::string input);
std::list<std::string>* parse_by_sticks(std::string input);
std::list<int>* find_numbers(std::string input);


#endif
