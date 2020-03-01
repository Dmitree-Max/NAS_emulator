

#include "Box.h"

extern std::list<std::thread> global_processes;

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
	int error = 30000000;
	std::string* addit = Socket_interactions::get_additional_fields(socket, req->cnt * note_length);
	int sussecc_count = 0;
	int displacement = 0;
	for (int i = 0; i < req->cnt; i++)
	{
		if (this->load > this->max_load)
		{
			answer->header = 30000003;
			answer->cnt = sussecc_count;
			return;
		}
		int src = std::stoi(addit->substr(0 + displacement, 8 + displacement) , 0, 16);
		int dst = std::stoi(addit->substr(8 + displacement, 16 + displacement) , 0, 16);
		displacement += 16;
		Disk* src_disk = this->find_device_by_sym(src);
		Disk* dst_disk = this->find_device_by_sym(dst);
		if (src_disk==nullptr or dst_disk==nullptr)
		{
			error = 30000002;
		}
		else
		{
			bool suc1 = dst_disk->make_coping_to_this_disk(src);
			bool suc2 = src_disk->make_coping_from_this_disk(dst);

			if (suc1 && suc2 == true)
			{
				if (error == 30000000)
					error = 30000001;
			}
			else
			{
				sussecc_count += 1;
			}
		}
	}
	answer->header = error;
	answer->cnt = sussecc_count;
	answer->cmd = 3;
	return;
}


void Box::activate_local_coping(int socket, Request* req, Answer* answer)
{
	int note_length = 8; //8 bytes, src, dst
	int error = 30000000;
	std::string* addit = Socket_interactions::get_additional_fields(socket, req->cnt * note_length);
	int sussecc_count = 0;
	int displacement = 0;
	for (int i = 0; i < req->cnt; i++)
	{
		if (this->load > this->max_load)
		{
			answer->header = 30000003;
			answer->cnt = sussecc_count;
			return;
		}
		int src = std::stoi(addit->substr(0 + displacement, 8 + displacement) , 0, 16);
		int dst = std::stoi(addit->substr(8 + displacement, 16 + displacement) , 0, 16);
		displacement += 16;
		Disk* src_disk = this->find_device_by_sym(src);
		Disk* dst_disk = this->find_device_by_sym(dst);
		if (src_disk==nullptr or dst_disk==nullptr)
		{
			error = 30000002;
		}
		else
		{
			src_disk->set_owner(this);
			dst_disk->set_owner(this);

			src_disk->lock_mutex();
			dst_disk->lock_mutex();
			if (src_disk->is_it_src_to(dst) && dst_disk->is_it_dst_to(src))
			{
				if (error == 30000000)
					error = 30000001;
			}
			else
			{
				global_processes.push_back(std::thread(Disk::start_coping_to, src_disk, src, 10));
				global_processes.push_back(std::thread(Disk::start_coping_from, dst_disk, dst, 10));
				sussecc_count += 1;
			}
			src_disk->free_mutex();
			dst_disk->free_mutex();
		}
	}
	answer->header = error;
	answer->cnt = sussecc_count;
	answer->cmd = 5;
	return;


}

Disk* Box::find_device_by_sym(int sym)
{
	for(auto disk : *(this->disks))
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


std::string Box::find_all_coping(Request* req, Answer* ans)
{
	std::string* addit = new std::string;
	std::list<std::string> copings;
	for(auto diskinf : *(this->disks))
	{
		int dst = diskinf.disk->get_coping_to();
		if (dst != 0)
		{
			std::string current_str = expand_to_byte(diskinf.sym) + expand_to_byte(dst);
			copings.push_back(current_str);
		}
	}

	int note_counter = 0;
	int start = req->start;
	int amount = req->cnt;

	std::cout << "Len: " << copings.size() << std::endl;
	for (auto note : copings)
	{
		if (note_counter > amount)
		{
			break;
		}
		if (note_counter >= start)
		{
			*addit += note;
		}
		note_counter += 1;
	}
	ans->cmd = 4;
	ans->cnt = note_counter;
	ans->header = 30000000;
	std::cout << "amount: " << amount << std::endl;
	return *addit;
}






























