
#ifndef MAIN_FUNCTIONS_H_
#define MAIN_FUNCTIONS_H_


#include <iostream>
#include <string>
#include <list>
#include "../NAS_client/NAS_client.h"
#include "Structure.h"


extern Structure* structure;
struct first_command_answer
{
	int box;
	int device;
	int sym;
	int number;
	int box_version;
};

struct Request {
	int device;
	int header;
	int flags;
	int start;
	int cnt;
	int cmd;
	int fmt;
	int mhop;
};

void get_box_and_disks_configuration();
bool parse_answer(std::string* src, struct Answer* ans);
std::string make_simple_command(int device, int cnt, int cmd);
bool parse_first_command_additional_fields(std::string* src, struct first_command_answer* answer);
bool parse_second_command_additional_fields(std::string* src, std::list<int>* answer);
struct hash_table_node* find_box(int number);
std::string to_hex(int number);
int make_local_pairs(struct Request* req, std::string* additional);
int can_make_local_pair(int box, int src_sym, int dst_sym);
void get_other_disks();
bool parse_request(std::string* src, struct Request* req);

// returns 0 if request valuable, else returns number of error, if can't be sure put in additional request which should be done
int is_request_valuable(std::string request, std::list<std::string>* additional);

void print_structure();

int activate_local_pairs(struct Request* req, std::string* additional);

int can_activate_local_pair(int box, int src_sym, int dst_sym);

int remove_local_pairs(struct Request* req, std::string* additional);

int can_remove_local_pair(int box, int dst_sym);

int make_remote_pairs(struct Request* req, std::string* additional);

int can_make_remote_pair(int group, int src_box, int dst_box, int src_sym, int dst_sym);

int remove_remote_pairs(struct Request* req, std::string* additional);

int can_remove_remote_pair(int group, int src_box, int dst_box, int src_sym, int dst_sym);

int make_groups(struct Request* req, std::string* additional);

int can_make_group(int src_box, int dst_box, int src_group);

int delete_groups(struct Request* req, std::string* additional);

int can_delete_group(int group);


#endif
