

#ifndef STRUCTURE_H_
#define STRUCTURE_H_

#include <list>

struct Src_disk_info
{
	int group;
	int dst_sym;
	bool active;
};


struct Group
{
	int number;
	int src_box;
	int dst_box;
	std::list<std::pair<std::pair<int, int>, bool> >* devaices_pairs; // src sym, dst sym, is active
};


struct hash_table_node
{
	int box_number;
	std::list<int >* disks;
	std::list<std::pair<int, int> >* gatekeepers;       // first is device global number, second is sym
};

class Structure {
	std::list<std::pair<int, int> >* disks_boxes;  		// first is device global number, second is box. Here are only gate keepers
	std::list<std::pair<int, int*> >* boxes_dst_disks; 	// boxes and there disks arrays
	std::list<struct Group*>* groups;
	// boxes and there disks with link to list of this disk as source info
	std::list<std::pair<int, std::list<struct Src_disk_info*>** > >* boxes_src_disks; //array of pointers to lists

	std::list<struct hash_table_node*>* DSS;             // physical structure of DDS
public:
	Structure();
	std::list<struct hash_table_node*>* get_DSS();
	virtual ~Structure();
	int find_box_by_device(int number);
	int is_it_src_to(int box, int group, int src_sym, int dst_sym);
	int is_it_dist(int box, int dst_sym);
	void make_pair(int src_box, int dst_box, int group, int src_sym, int dst_sym);
	void make_remote_pair(int src_box, int dst_box, int group, int src_sym, int dst_sym);
	void remove_remote_pair(int src_box, int dst_box, int group, int src_sym, int dst_sym);
	void delete_pair(int box, int group, int src_sym, int dst_sym);
	struct Group* find_group(int group);
	int is_there_remote_pair(int group, int src_sym, int dst_sym);
	int find_box_by_group(int box, int group);
	bool is_there_such_box(int box);
	int calc_amount_groups_with_box(int box);
	void make_group(int group, int src_box, int dst_box);
	int delete_group(int group);
	bool is_there_active_coping(struct Group* group);
	void initialize_full_structure();
};



#endif
