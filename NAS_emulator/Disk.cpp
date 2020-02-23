

#include "Disk.h"

Disk::Disk(int arg_number) {
	number = arg_number;
	is_coping_to = 0;
	is_coping_from = 0;
}

Disk::~Disk() {
}

int Disk::get_number() {
	return this->number;
}

void Disk::start_coping_to(Box* owner, int dst, int time)
{
	std::lock_guard<std::mutex> lock(this->disk_mutex);
	this->coping_to = dst;
	sleep(time);
	owner->decreese_load;
	th->coping_to = 0;
	return;
}


void Disk::start_coping_from(Box* owner, int src, int time)
{
	std::lock_guard<std::mutex> lock(this->disk_mutex);
	this->coping_from = src;
	sleep(time);
	owner->decreese_load;
	th->coping_from = 0;
	return;
}



