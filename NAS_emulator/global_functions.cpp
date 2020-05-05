
#include "global_functions.h"


void print_log(int num, ...)
{
	log_file.open("log.txt");
    va_list args;
    va_start(args, num);
    while (num--) {
    	log_file << va_arg(args, unsigned);
    }
    va_end(args);
	log_file.close();
}
