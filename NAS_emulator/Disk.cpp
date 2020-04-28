

#include "Disk.h"

Disk::Disk(int arg_number) {
	number = arg_number;
	src_of_coping_to_this_disk = 0;
	coping_from_this_disk = new std::list<std::pair<int, int> >; //group, dst
	active_coping = false;
	owner = nullptr;
	group = 0;
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


void Disk::add_coping_from_this_disk(int group, int dst){
	this->coping_from_this_disk->push_back(std::make_pair(group,dst));
}


std::list<std::pair<int, int> >* Disk::get_coping_list()
{
	return this->coping_from_this_disk;
}


int Disk::get_group()
{
	return this->group;
}


void Disk::start_coping(Disk* th, int time)
{
	//std::lock_guard<std::mutex> lock(th->disk_mutex);
	th->active_coping = true;
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
	th->active_coping = false;
	return;
}


void Disk::set_owner(Box* owner)
{
	this->owner = owner;
	return;
}


void Disk::remove_pair_with(int group, int dst)
{
	for(auto iter = this->coping_from_this_disk->begin(); iter != this->coping_from_this_disk->end(); iter++)
	{
		if ((iter->first == group) and (iter->second == dst))
		{
			this->coping_from_this_disk->erase(iter);
			break;
		}
	}
}


int Disk::get_coping_to()
{
	return this->src_of_coping_to_this_disk;
}


bool Disk::make_coping_to_this_disk(int group, int src)
{
	this->group = group;
	if (this->disk_mutex.try_lock() == true)
	{
		bool result;
		if (this->src_of_coping_to_this_disk == 0)
		{
			this->src_of_coping_to_this_disk = src;
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


bool Disk::is_it_src_to(int dst)
{
	return (this->src_of_coping_to_this_disk == dst);
}


bool Disk::is_it_active_coping(){
	return this->active_coping;
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




