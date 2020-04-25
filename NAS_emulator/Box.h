

#ifndef BOX_H_
#define BOX_H_
#include "Disk.h"
#include "interior_formats.h"
#include "Socketinteractions.h"



#include <fstream>
#include <list>


extern int BOX_MAX_LOAD;
extern int LOCAL_COPING_TIME;
class Disk;

extern std::list<std::thread> global_processes;

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
	std::string find_all_distance_coping(Request* req, Answer* ans);
	void activate_local_coping(int socket, Request* req, Answer* answer);
	void activate_track_local_coping(int socket, Request* req, Answer* answer);
	void decreese_load();
	std::string find_all_local_coping(Request* req, Answer* ans);
	std::list<struct Disk_info>* get_disks();
	Box(int number);
	~Box();
};

#endif /* BOX_H_ */
