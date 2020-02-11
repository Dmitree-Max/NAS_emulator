

#ifndef BOX_H_
#define BOX_H_
#include "Disk.h"
#include "interior_formats.h"

#include <list>


struct Disk_info
{
	int sym;
	Disk* disk;
	friend class Box;
};

class Box {
private:
	int number;
	int version;
	std::list<struct Disk_info>* disks;
public:
	std::string is_device_in_box(int device, Answer* answer);
	void set_disks(std::list<struct Disk_info>* disks);
	Box(int number);
	~Box();
};

#endif /* BOX_H_ */
