

#include "Disk.h"

Disk::Disk(int arg_number) {
	number = arg_number;
	coping_to = 0;
	coping_from = 0;
	owner = nullptr;
}

Disk::~Disk() {
}

int Disk::get_number() {
	return this->number;
}


void Disk::set_coping_to(int dst)
{
	this->coping_to = dst;
	return;
}
void Disk::start_coping_to(Disk* th, int dst, int time)
{
	std::lock_guard<std::mutex> lock(th->disk_mutex);
	th->coping_to = dst;
	sleep(time);
	if (th->owner != nullptr)
	{
		th->owner->decreese_load();
		std::cout <<"process stoped" << std::endl;
	}
	else
	{
		std::cout <<" i have no owner" << std::endl;
	}
	th->coping_to = 0;
	return;
}


void Disk::start_coping_from(Disk* th, int src, int time)
{
	std::lock_guard<std::mutex> lock(th->disk_mutex);
	th->coping_from = src;
	sleep(time);
	if (th->owner!=nullptr)
	{
		th->owner->decreese_load();
		std::cout <<"process stoped" << std::endl;
	}
	else
	{
		std::cout <<" i have no owner" << std::endl;
	}
	std::cout << "coping finished" << std::endl;
	th->coping_from = 0;
	return;
}


void Disk::set_owner(Box* owner)
{
	this->owner = owner;
	return;
}


int Disk::get_coping_to()
{
	return this->coping_to;
}

