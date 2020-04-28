
#ifndef DISK_H_
#define DISK_H_

#include <mutex>
#include <iostream>
#include <thread>
#include <unistd.h>
#include "Box.h"
#include <list>


class Box;

class Disk {
	int number;
	std::mutex disk_mutex;
	std::list<std::pair<int, int> >* coping_from_this_disk;
	int src_of_coping_to_this_disk;
	bool active_coping;
	int group;
	Box* owner;
public:
	Disk(int number);
	~Disk();
	int get_number();
	bool can_be_dist();
	void static start_coping(Disk* th, int time);

	void set_owner(Box* parent);
	void add_coping_from_this_disk(int group, int dst);
	bool is_it_active_coping();
	int get_coping_to();
	int get_group();
	std::list<std::pair<int, int> >* get_coping_list();
	void set_coping_to(int dst);
	bool make_coping_to_this_disk(int group, int src);
	bool is_it_src_to(int dst);
	bool is_it_dst_to(int src);
	void remove_from_pair();
	void lock_mutex();
	void free_mutex();
};

#endif
