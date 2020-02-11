

#include "Box.h"

Box::Box(int input_number) {
	disks = nullptr;
	version = 0;
	number = input_number;
}

Box::~Box() {
	// TODO Auto-generated destructor stub
}

std::string* Box::is_device_in_box(int device, Answer* answer){
	bool is_device_in_box = false;
	int device_sym = 0;
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
	std::string* add_answer = new std::string;
	if (is_device_in_box)
	{
		answer->header = 3000000;
		answer->flags  = 0;
		answer->start  = 0;
		answer->cnt    = 1;
		answer->cmd    = 1;
		answer->fmt    = 0;
		*add_answer = expand_to_byte(this->number) \
				+ expand_to_byte(device) + expand_to_byte(device_sym) + expand_to_byte(0) +\
				expand_to_byte(this->version);
	}
	else
	{
		answer->header = 3000008;
		answer->flags  = 0;
		answer->start  = 0;
		answer->cnt    = 1;
		answer->cmd    = 1;
		answer->fmt    = 0;
	}
	return add_answer;
}
