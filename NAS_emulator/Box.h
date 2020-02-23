

#ifndef BOX_H_
#define BOX_H_
#include "Disk.h"
#include "interior_formats.h"
#include "Socterinteractions.h"

#include <list>


struct Disk_info
{
	int sym;
	Disk* disk;
	friend class Box;
};

class Box {
private:
	int load;
	int number;
	int version;
	int max_load;
	std::list<struct Disk_info>* disks;
public:
	std::string make_answer_is_device_in_box(int device, Answer* answer);
	bool find_if_device_in_box(int device, Answer* answer, int& sym);
	Disk* find_device_by_sym(int sym);
	void set_disks(std::list<struct Disk_info>* disks);
	void make_local_coping(int socket, Request* req, Answer* answer);
	void decreese_load();
	std::list<struct Disk_info>* get_disks();
	Box(int number);
	~Box();
};

#endif /* BOX_H_ */
