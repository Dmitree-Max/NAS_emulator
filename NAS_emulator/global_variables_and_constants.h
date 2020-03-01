#ifndef GLOBAL_VARIABLES_AND_CONSTANTS
#define GLOBAL_VARIABLES_AND_CONSTANTS
#include <thread>
#include <list>
#include "Requesthandler.h"

struct Box_info;

std::list<Box_info> global_boxes;
std::list<std::thread> global_processes;
int LOCAL_COPING_TIME = 10;
int BOX_MAX_LOAD = 10;
int WORKING_PORT = 5060;

#endif
