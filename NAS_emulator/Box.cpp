

#include "Box.h"

Box::Box(int input_number) {
	disks = new  std::list<struct Disk_info>;
	version = 0;
	number = input_number;
	load = 0;
	max_load = 10;
}

Box::~Box() {
	// TODO Auto-generated destructor stub
}


bool Box::find_if_device_in_box(int device, Answer* answer, int& device_sym){
	bool is_device_in_box = false;
	if (this->disks!=nullptr)
	{
		for (auto& device_info : *(this->disks))
		{
			if (device_info.disk->get_number() == device)
			{
				is_device_in_box = true;
				device_sym = device_info.sym;
				break;
			}
		}
	}
	return is_device_in_box;

}


std::string Box::make_answer_is_device_in_box(int device, Answer* answer){
	int device_sym = 0;
	bool is_device_in_box = find_if_device_in_box(device, answer, device_sym);
	std::string add_answer = "";
	if (is_device_in_box)
	{
		answer->header = 30000000;
		answer->flags  = 0;
		answer->start  = 0;
		answer->cnt    = 1;
		answer->cmd    = 1;
		answer->fmt    = 0;
		add_answer = expand_to_byte(this->number) \
				+ expand_to_byte(device) + expand_to_byte(device_sym) + expand_to_byte(0) +\
				expand_to_byte(this->version);
	}
	else
	{
		answer->header = 30000008;
		answer->flags  = 0;
		answer->start  = 0;
		answer->cnt    = 0;
		answer->cmd    = 1;
		answer->fmt    = 0;
	}
	return add_answer;
}

void Box::set_disks(std::list<struct Disk_info>* disks)
{
	this->disks = disks;
	return;
}

std::list<struct Disk_info>* Box::get_disks()
{
	return this->disks;
}


void Box::make_local_coping(int socket, Request* req, Answer* answer)
{
	int note_length = 8; //8 bytes, src, dst
	int error = 0;
	std::string addit = Socket_interactions::get_additional_fields(socket, req->cnt * note_length);
	int sussecc_count = 0;
	int displacement = 0;
	for (int i = 0; i < req->cnt; i++)
	{
		if (this->load > this->max_load)
		{
			answer->header = 3;
			answer->cnt = sussecc_count;
			return;
		}
		int src = std::stoi(addit.substr(0 + displacement, 8 + displacement) , 0, 16);
		int dst = std::stoi(addit.substr(8 + displacement, 16 + displacement) , 0, 16);
		displacement += 16;
		Disk* src_disk = this->find_device_by_sym(src);
		Disk* dst_disk = this->find_device_by_sym(dst);
		if (src_disk==nullpt or dst_disk==nullptr)
		{
			error = 2;
		}
		else
		{
			std::thread(Disk::coping_to, this, dst_disk, 10);
			std::thread(Disk::coping_from, this, src_disk, 10);
		}
	}
	answer->header = error;
	answer->cnt = sussecc_count;
	return;
}


Disk* Box::find_device_by_sym(int sym)
{
	for(auto disk : this->disks)
	{
		if (disk.sym == sym)
			return disk.disk;
	}
	return nullptr;
}



void Box::decreese_load()
{
	this->load--;
	return;
}









