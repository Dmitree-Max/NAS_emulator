

#ifndef BOX_H_
#define BOX_H_
#include "Disk.h"
#include "interior_formats.h"
#include "Socketinteractions.h"
#include "Requesthandler.h"
#include "global_functions.h"


#include <list>


extern int BOX_MAX_LOAD;
extern int LOCAL_COPING_TIME;
extern std::ofstream log_file;
extern std::list<std::thread> global_processes;

class Disk;
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
	std::list<std::pair<std::pair<int, int>, int> >* groups;      // first is group number in this box and second box and second is box id
public:
	std::string make_answer_is_device_in_box(int device, Answer* answer);
	bool find_if_device_in_box(int device, Answer* answer, int& sym);
	Disk* find_device_by_sym(int sym);
	void make_group(int socket, Request* req, Answer* ans);
	Box* find_box_by_group(int group);
	void set_disks(std::list<struct Disk_info>* disks);
	void make_local_coping(int socket, Request* req, Answer* answer);
	void make_remote_coping(int socket, Request* req, Answer* answer);
	std::string get_all_devices(Request* req, Answer* answer);
	std::string find_all_distance_coping(Request* req, Answer* ans);
	void activate_local_coping(int socket, Request* req, Answer* answer);
	void activate_track_local_coping(int socket, Request* req, Answer* answer);
	void decreese_load();
	void delete_local_pair(int socket, Request* req, Answer* ans);
	void delete_remote_pair(int socket, Request* req, Answer* ans);
	void remove_group(int socket, Request* req, Answer* ans);
	void delete_group(int group);
	bool is_active_coping_in_group(int group);
	std::string find_all_local_coping(Request* req, Answer* ans);
	int get_group_ammount();
	std::list<struct Disk_info>* get_disks();
	int get_group_on_another_side(int group);
	Box(int number);
	~Box();
};

#endif /* BOX_H_ */
