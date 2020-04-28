

#include "Box.h"

std::list<std::thread> global_processes;

Box::Box(int input_number) {
	disks = new  std::list<struct Disk_info>;
	groups = new std::list<std::pair<int, int> >;
	version = 0;
	number = input_number;
	load = 0;
	max_load = BOX_MAX_LOAD;
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


int Box::get_group_on_another_side(int group)
{
	int box_group_with_name = 0;
	for(auto group_info : *(this->groups))
	{
		if (group_info.first == group)
		{
			box_group_with_name = group_info.second;
			break;
		}
	}
	if (box_group_with_name != 0)
	{
		Box* box = Request_handler::find_box_by_name(box_group_with_name);
		if (box == nullptr)
		{
			return 0;
		}
		else
		{
			for(auto group_info : *(box->groups))
			{
				if (group_info.first == this->number)
				{
					return group_info.second;
				}
			}
		}
	}
	return 0;
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
			break;
		}
		int src = std::stoi(addit->substr(0 + displacement, 8) , 0, 16);
		int dst = std::stoi(addit->substr(8 + displacement, 8) , 0, 16);
		displacement += 16;
		Disk* src_disk = this->find_device_by_sym(src);
		Disk* dst_disk = this->find_device_by_sym(dst);
		if (src_disk==nullptr or dst_disk==nullptr)
		{
			error = 30000002;
			break;
		}
		else
		{
			bool already_in_pair = src_disk->is_it_src_to(dst);
			if (already_in_pair){
				error = 30000001;
				break;
			}
			bool can_be_target = dst_disk->can_be_dist();
			if (can_be_target == false)
			{
					error = 30000004;
					break;
			}
			else
			{
				dst_disk->make_coping_to_this_disk(0, src);
				src_disk->add_coping_from_this_disk(0, dst);
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

    std::ofstream log;
    log.open("log.txt");
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
			log << "бокс " << this->number << " перегружен" << std::endl;
			break;
		}
		int src = std::stoi(addit->substr(0 + displacement, 8) , 0, 16);
		int dst = std::stoi(addit->substr(8 + displacement, 8) , 0, 16);
		displacement += 16;
		Disk* src_disk = this->find_device_by_sym(src);
		Disk* dst_disk = this->find_device_by_sym(dst);
		if (src_disk==nullptr or dst_disk==nullptr)
		{
			error = 30000002;
			log << "девайсы недоступны" << std::endl;
			break;
		}
		else
		{
			src_disk->set_owner(this);
			dst_disk->set_owner(this);

			src_disk->lock_mutex();
			dst_disk->lock_mutex();
			if (!dst_disk->is_it_dst_to(src))
			{
				error = 30000001;
				break;
			}
			else
			{
				global_processes.push_back(std::thread(Disk::start_coping, src_disk, LOCAL_COPING_TIME));
				global_processes.push_back(std::thread(Disk::start_coping, dst_disk, LOCAL_COPING_TIME));
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


void Box::activate_track_local_coping(int socket, Request* req, Answer* answer)
{
	int note_length = 24; //24 bytes, src, dst devises, src track, src range, dst track, dst range 4 bytes each
	int error = 30000000;
	std::string* addit = Socket_interactions::get_additional_fields(socket, req->cnt * note_length);
	int sussecc_count = 0;
	int displacement = 0;
	for (int i = 0; i < req->cnt; i++)
	{
		if (this->load > this->max_load)
		{
			answer->header = 30000003;
			break;
		}
		int src = std::stoi(addit->substr(0 + displacement, 8) , 0, 16);
		int dst = std::stoi(addit->substr(8 + displacement, 8) , 0, 16);
		displacement += 48;
		Disk* src_disk = this->find_device_by_sym(src);
		Disk* dst_disk = this->find_device_by_sym(dst);
		if (src_disk==nullptr or dst_disk==nullptr)
		{
			error = 30000002;
			break;
		}
		else
		{
			src_disk->set_owner(this);
			dst_disk->set_owner(this);

			src_disk->lock_mutex();
			dst_disk->lock_mutex();
			if (!dst_disk->is_it_dst_to(src))
			{
				error = 30000001;
				break;
			}
			else
			{
				global_processes.push_back(std::thread(Disk::start_coping, src_disk,  LOCAL_COPING_TIME));
				global_processes.push_back(std::thread(Disk::start_coping, dst_disk,  LOCAL_COPING_TIME));
				sussecc_count += 1;
			}
			src_disk->free_mutex();
			dst_disk->free_mutex();
		}
	}
	answer->header = error;
	answer->cnt = sussecc_count;
	answer->cmd = 6;
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


std::string Box::find_all_local_coping(Request* req, Answer* ans)
{
	std::string* addit = new std::string;
	std::list<std::string> copings;
	for(auto diskinf : *(this->disks))
	{
		int src = diskinf.disk->get_coping_to();
		int group = diskinf.disk->get_group();
		if ((src != 0) and (group == 0))
		{
			std::string current_str = expand_to_byte(src) + expand_to_byte(diskinf.sym);
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


void Box::delete_local_pair(int socket, Request* req, Answer* answer){
	int note_length = 4; // dst
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
			break;
		}
		int dst = std::stoi(addit->substr(displacement, 4) , 0, 16);
		displacement += 4;
		Disk* dst_disk = this->find_device_by_sym(dst);
		if (dst_disk==nullptr)
		{
			error = 30000002;
			break;
		}
		else
		{
			bool coping_this_moment = dst_disk->is_it_active_coping();
			if (coping_this_moment){
				error = 30000001;
				break;
			}
			dst_disk->remove_from_pair();
			sussecc_count += 1;
		}
	}
	answer->header = error;
	answer->cnt = sussecc_count;
	answer->cmd = 7;
	return;
}


std::string Box::find_all_distance_coping(Request* req, Answer* ans)
{
	std::string* addit = new std::string;
	std::list<std::string> copings;
	for(auto diskinf : *(this->disks))
	{
		int src = diskinf.disk->get_coping_to();
		int group = diskinf.disk->get_group();
		if ((src != 0) and (group != 0))
		{
			std::string current_str = expand_to_byte(src) + expand_to_byte(diskinf.disk->get_number())
					+ expand_to_byte(this->get_group_on_another_side(group)) + expand_to_byte(group);
			copings.push_back(current_str);
		}
	}

	for(auto diskinf : *(this->disks))
	{
		for(auto dst_element : *(diskinf.disk->get_coping_list()))
		{
			int dst = dst_element.second;
			int group = dst_element.first;
			if ((dst != 0) and (group != 0))
			{
				std::string current_str = expand_to_byte(diskinf.disk->get_number()) + expand_to_byte(dst) + expand_to_byte(group)
						+ expand_to_byte(this->get_group_on_another_side(group));
				copings.push_back(current_str);
			}
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
	ans->cmd = 2;
	ans->cnt = note_counter;
	ans->header = 30000000;
	std::cout << "amount: " << amount << std::endl;
	return *addit;
}



























