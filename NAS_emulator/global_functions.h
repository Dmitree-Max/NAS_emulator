
#ifndef GLOBAL_FUNCTIONS_H_
#define GLOBAL_FUNCTIONS_H_

#include <fstream>
#include <stdarg.h>
#include <list>


extern std::list<struct Box_info> global_boxes;
extern std::ofstream log_file;

void print_log(int num, ...);


#endif
