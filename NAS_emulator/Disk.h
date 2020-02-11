
#ifndef DISK_H_
#define DISK_H_

#include <mutex>
#include <iostream>


class Disk {
	int number;
	std::mutex disk_mutex;
public:
	Disk(int number);
	~Disk();
	int get_number();
};

#endif
