
#include "Structure.h"


Structure* structure = new Structure();


Structure::Structure() {
	// TODO Auto-generated constructor stub

}

Structure::~Structure() {
	// TODO Auto-generated destructor stub
}

int Structure::find_box_by_device(int number)
{
	for (auto disk : this->disks_boxes)
	{
		if (disk.first == number)
		{
			return disk.second;
		}
	}
	return 0;
}


int Structure::is_it_dist(int box, int dst_sym)
{
	for (auto disk : this->boxes_dst_disks)
	{
		if (disk.first == box)
		{
			return disk.second[dst_sym];
		}
	}
	return -1;
}


std::list<struct hash_table_node*>* Structure::get_DSS()
{
	return this->DSS;
}


int Structure::is_it_src_to(int box, int group, int src_sym, int dst_sym)
{
	for (auto current_box : this->boxes_src_disks)
	{
		if (current_box.first == box)
		{
			std::list<struct Src_disk_info>* diks_as_srcs = current_box.second[src_sym];
			for (struct Src_disk_info link : (*diks_as_srcs))
			{
				if ((link.group == group) and (link.dst_sym == dst_sym))
				{
					if (link.active == true)
						return 4;
					return 1;
				}
			}
			return 0;
		}
	}
	return -1;
}


void Structure::make_pair(int src_box, int dst_box, int group, int src_sym, int dst_sym)
{
	for (auto current_box : this->boxes_src_disks)
	{
		if (current_box.first == src_box)
		{
			std::list<struct Src_disk_info>* diks_as_srcs = current_box.second[src_sym];
			struct Src_disk_info* new_link = new struct Src_disk_info;
			new_link->active = false;
			new_link->group = group;
			new_link->dst_sym = dst_sym;
			diks_as_srcs->push_back(*new_link);
			break;
		}
	}
	for (auto current_box : structure->boxes_dst_disks)
	{
		if (current_box.first == dst_box)
		{
			current_box.second[dst_sym] = src_sym;
		}
	}
}


void Structure::make_remote_pair(int src_box, int dst_box, int group, int src_sym, int dst_sym)
{
	struct Group* cur_group = find_group(group);
	if (cur_group != nullptr)
	{
		cur_group->devaices_pairs->push_back(std::make_pair(std::make_pair(src_sym, dst_sym), false));
		for (auto current_box : structure->boxes_dst_disks)
		{
			if (current_box.first == dst_box)
			{
				current_box.second[dst_sym] = src_sym;
			}
		}
	}
}


void Structure::remove_remote_pair(int src_box, int dst_box, int group, int src_sym, int dst_sym)
{
	struct Group* cur_group = find_group(group);
	if (cur_group != nullptr)
	{
		//cur_group->devaices_pairs->push_back(std::make_pair(std::make_pair(src_sym, dst_sym), false));
		for (auto iter = cur_group->devaices_pairs->begin(); iter != cur_group->devaices_pairs->end(); iter++)
		{
			if (iter->first.first == src_sym and iter->first.second == dst_sym)
			{
				cur_group->devaices_pairs->erase(iter);
				break;
			}
		}
		for (auto current_box : structure->boxes_dst_disks)
		{
			if (current_box.first == dst_box)
			{
				current_box.second[dst_sym] = 0;
			}
		}
	}
}

void Structure::delete_pair(int box, int group, int src_sym, int dst_sym)
{
	for (auto current_box : this->boxes_src_disks)
	{
		if (current_box.first == box)
		{
			std::list<struct Src_disk_info>* diks_as_srcs = current_box.second[src_sym];
			for (auto iter = diks_as_srcs->begin(); iter != diks_as_srcs->end(); iter++)
			{
				if (iter->group == group and iter->dst_sym == dst_sym)
				{
					diks_as_srcs->erase(iter);
					break;
				}
			}
			break;
		}
	}
	for (auto current_box : structure->boxes_dst_disks)
	{
		if (current_box.first == box)
		{
			current_box.second[dst_sym] = src_sym;
		}
	}
}


struct Group* Structure::find_group(int group)
{
	for (struct Group* gr : (*this->groups))
	{
		if (gr->number == group)
			return gr;
	}
	return nullptr;
}


int Structure::is_there_remote_pair(int group, int src_sym, int dst_sym)
{
	struct Group* cur_group = find_group(group);
	if (cur_group == nullptr)
		return -1;
	for (auto pair : (*cur_group->devaices_pairs))
	{
		if (pair.first.first == src_sym and pair.first.second == dst_sym)
		{
			if (pair.second == true)
				return 4;
			else
				return 1;
		}

	}
	return 0;
}


int Structure::find_box_by_group(int box, int group)
{
	for (struct Group* gr : (*this->groups))
	{
		if (gr->number == group)
		{
			if (gr->src_box == box)
				return gr->dst_box;
			if (gr->dst_box == box)
				return gr->src_box;
			break;
		}
	}
	return 0;
}


bool Structure::is_there_such_box(int box)
{
	for (auto node :(*this->DSS))
	{
		if (node->box_number == box)
			return true;
	}
	return false;
}


int Structure::calc_amount_groups_with_box(int box)
{
	int result = 0;
	for (auto gr : (*this->groups))
	{
		if (gr->src_box == box or gr->dst_box == box)
			result += 1;
	}
	return result;
}


void Structure::make_group(int group, int src_box, int dst_box)
{
	std::list<std::pair<std::pair<int, int>, bool> >* new_pairs_list= new std::list<std::pair<std::pair<int, int>, bool> >;
	struct Group* new_group = new struct Group;
	new_group->devaices_pairs = new_pairs_list;
	new_group->dst_box = dst_box;
	new_group->src_box = src_box;
	new_group->number  = group;
	this->groups->push_back(new_group);
}

int Structure::delete_group(int group)
{
	for (auto iter = this->groups->begin(); iter != this->groups->end(); iter++)
	{
		if (is_there_active_coping(*iter))
		{
			return -2;
		}
		else
		{
			this->groups->erase(iter);
			delete(*iter);
			return 0;
		}
	}
	return -1;
}


bool Structure::is_there_active_coping(struct Group* group)
{
	for (auto link : (*group->devaices_pairs))
	{
		if (link.second == true)
			return true;
	}
	return false;
}
