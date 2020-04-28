

#include "Disk.h"

Disk::Disk(int arg_number) {
	number = arg_number;
	src_of_coping_to_this_disk = 0;
	std::list<std::pair<bool, int> >*  coping_from_this_disk = new std::list<std::pair<bool, int> >;
	active_coping_to = 0;
	active_coping_from = 0;
	owner = nullptr;
	sym_coping = true;
}

Disk::~Disk() {
}

int Disk::get_number() {
	return this->number;
}


void Disk::set_coping_to(int dst)
{
	this->src_of_coping_to_this_disk = dst;
	return;
}


void Disk::add_coping_from_this_disk(bool is_sym_coping, int dst){
	this->coping_from_this_disk->push_back(std::make_pair(is_sym_coping, dst));
}

void Disk::start_coping_to(Disk* th, int dst, int time)
{
	//std::lock_guard<std::mutex> lock(th->disk_mutex);
	th->active_coping_to = dst;
	sleep(time);
	if (th->owner != nullptr)
	{
		th->owner->decreese_load();
		std::cout <<"process stopped" << std::endl;
	}
	else
	{
		std::cout <<" i have no owner" << std::endl;
	}
	th->active_coping_to = 0;
	return;
}


void Disk::start_coping_from(Disk* th, int src, int time)
{
	//std::lock_guard<std::mutex> lock(th->disk_mutex);
	th->active_coping_from = src;
	sleep(time);
	if (th->owner!=nullptr)
	{
		th->owner->decreese_load();
		std::cout <<"process stopped" << std::endl;
	}
	else
	{
		std::cout <<" i have no owner" << std::endl;
	}
	std::cout << "coping finished" << std::endl;
	th->active_coping_from = 0;
	return;
}


void Disk::set_owner(Box* owner)
{
	this->owner = owner;
	return;
}


int Disk::get_coping_to()
{
	return this->src_of_coping_to_this_disk;
}


bool Disk::make_local_coping_to_this_disk(int src)
{
	if (this->disk_mutex.try_lock() == true)
	{
		bool result;
		if (this->src_of_coping_to_this_disk == 0)
		{
			this->src_of_coping_to_this_disk = src;
			this->sym_coping = true;
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


bool Disk::can_be_dist()
{
	return this->src_of_coping_to_this_disk != 0;
}


bool Disk::get_coping_format()
{
	return this->sym_coping;
}

bool Disk::make_local_coping_from_this_disk(int dst)
{
	if (this->disk_mutex.try_lock() == true)
	{
		bool result;
		if (this->coping_from_this_disk == 0 && this->src_of_coping_to_this_disk == 0)
		{
			this->src_of_coping_to_this_disk = dst;
			this->sym_coping = true;
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
	return (this->src_of_coping_to_this_disk == dst);
}


bool Disk::is_it_active_coping(){
	return (this->active_coping_from != 0) or (this->active_coping_to != 0);
}


void Disk::remove_from_pair(){
		this->src_of_coping_to_this_disk = 0;
		this->coping_from_this_disk->clear();
}


bool Disk::is_it_dst_to(int src)
{
	return (this->src_of_coping_to_this_disk == src);
}

void Disk::lock_mutex()
{
	this->disk_mutex.lock();
}

void Disk::free_mutex()
{
	this->disk_mutex.unlock();
}




