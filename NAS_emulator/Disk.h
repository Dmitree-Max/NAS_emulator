
#ifndef DISK_H_
#define DISK_H_

#include <mutex>
#include <iostream>
#include <thread>
#include <unistd.h>
#include "Box.h"


class Box;

class Disk {
	int number;
	std::mutex disk_mutex;
	int coping_from;
	int coping_to;
	Box* owner;
	bool sym_coping;
public:
	Disk(int number);
	~Disk();
	int get_number();
	void static start_coping_from(Disk* th, int src, int time);
	void static start_coping_to(Disk* th, int dst, int time);
	void set_owner(Box* parent);
	int get_coping_to();
	void set_coping_to(int dst);
	bool make_local_coping_to_this_disk(int src);
	bool get_coping_format();
	bool make_local_coping_from_this_disk(int dst);
	bool is_it_src_to(int dst);
	bool is_it_dst_to(int src);
	void lock_mutex();
	void free_mutex();
};

#endif
