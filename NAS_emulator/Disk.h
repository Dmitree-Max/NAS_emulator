
#ifndef DISK_H_
#define DISK_H_

#include <mutex>
#include <iostream>
#include <thread>
#include <unistd.h>

class Disk {
	int number;
	std::mutex disk_mutex;
	int is_coping_from;
	int is_coping_to;
public:
	Disk(int number);
	~Disk();
	int get_number();
	void static start_coping_from(Box* owner, Disk* th, int src);
	void static start_coping_to(Box* owner, Disk* th, int dst);
};

#endif
