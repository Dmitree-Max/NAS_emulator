

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


bool Disk::make_coping_to_this_disk(int src)
{
	if (this->disk_mutex.try_lock() == true)
	{
		bool result;
		if (this->coping_from == 0 && this->coping_to == 0)
		{
			this->coping_from = src;
			result = true;
		}
		else
		{
			result = false;
		}
		this->disk_mutex.unlock();
		return result;
	}
	else
	{
		return false;
	}
}


bool Disk::make_coping_from_this_disk(int dst)
{
	if (this->disk_mutex.try_lock() == true)
	{
		bool result;
		if (this->coping_from == 0 && this->coping_to == 0)
		{
			this->coping_to = dst;
			result = true;
		}
		else
		{
			result = false;
		}
		this->disk_mutex.unlock();
		return result;
	}
	else
	{
		return false;
	}
}


bool Disk::is_it_src_to(int dst)
{
	return (this->coping_to == dst);
}


bool Disk::is_it_dst_to(int src)
{
	return (this->coping_from == src);
}

void Disk::lock_mutex()
{
	this->disk_mutex.lock();
}

void Disk::free_mutex()
{
	this->disk_mutex.unlock();
}




