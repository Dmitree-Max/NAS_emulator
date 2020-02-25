

#include "system_initialize.h"
extern std::list<struct Box_info> global_boxes;

int initialize_system(const char* filename)
{
	struct Init_obj* initializer = parse_file(filename);
	std::cout << *initializer;

	std::map<int, Disk*> disks;

	for (int number : *(initializer->disks))
	{
		Disk* current = new Disk(number);
		disks.insert({number, current});
	}

	for (struct About_box boxinfo : initializer->boxes)
	{
		Box* current = new Box(boxinfo.box_name);
		std::list<struct Disk_info>* temp = new std::list<struct Disk_info>;
		for (auto& x : *(boxinfo.disk_sym))
		{
			struct Disk_info* cur = new struct Disk_info;
			cur->sym  = x.second;
			cur->disk = disks[x.first];
			temp->push_back(*cur);
		}
		current->set_disks(temp);
		struct Box_info* new_box = new struct Box_info;
		new_box->box = current;
		new_box->box_name = boxinfo.box_name;
		global_boxes.push_back(*new_box);
	}
	return 0;
}


struct Init_obj* parse_file(const char* filename)
{
	struct Init_obj* result = new Init_obj;
	std::streampos size;
	char * memblock;

	std::ifstream file (filename, std::ios::in|std::ios::binary|std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, std::ios::beg);
		file.read (memblock, size);
		file.close();
	}
	else
	{
	    return result;
	}

	std::string text = std::string(memblock);
	free(memblock);
	std::list<std::string>* parsed_by_brackets = parse_by_brackets(text);

	std::string disks_text = parsed_by_brackets->front();
	parsed_by_brackets->pop_front();
	result->disks = find_numbers(disks_text);


	std::string boxes_text = parsed_by_brackets->front();
	//std::cout << boxes_text<< std::endl;
	parsed_by_brackets->pop_front();

	std::list<std::string>* parsed_by_sticks = parse_by_sticks(boxes_text);
	for (auto& box : *parsed_by_sticks)
	{
		//std::cout << box << std::endl;
		std::list<int>* all_digits = find_numbers(box);
		struct About_box* current = new struct About_box;
		current->box_name = all_digits->front();
		//std::cout << current->box_name << std::endl;
		all_digits->pop_front();

		std::list<std::pair<int, int>>* disks = new std::list<std::pair<int, int>>;
		while(all_digits->size() > 1)
		{
			disks->push_back(std::make_pair(all_digits->front(), *std::next(all_digits->begin())));
			all_digits->pop_front();
			all_digits->pop_front();
		}
		current->disk_sym = disks;
		result->boxes.push_back(*current);
	}

    return result;
}



std::list<std::string>* parse_by_brackets(std::string input)
{
	std::regex req ("\\{([^\\}]*)\\}");
	std::smatch matches;
	std::list<std::string>* result = new std::list<std::string>;
	while(std::regex_search(input, matches, req))
	{
		result->push_back(matches[1]);
		input = matches.suffix().str();
	}
	return result;
}



std::list<std::string>* parse_by_sticks(std::string input)
{
	std::regex req ("\\|([^|]*)\\|");
	std::smatch matches;
	std::list<std::string>* result = new std::list<std::string>;
	while(std::regex_search(input, matches, req))
	{
		result->push_back(matches[1]);
		input = matches.suffix().str();
	}
	return result;
}


std::list<int>* find_numbers(std::string input)
{
	std::regex req ("[0-9]+");
	std::smatch matches;
	std::list<int>* result = new std::list<int>;
	while(std::regex_search(input, matches, req))
	{
		//std::cout <<"kuku: "<< matches[1];
		result->push_back(std::stoi(matches[0]));
		input = matches.suffix().str();
	}
	return result;


}


std::ostream& operator << (std::ostream& stm, const struct Init_obj& init)
{
	stm << "disks:  ";
	for (auto& disk : *(init.disks))
	{
		stm << disk << "  ";
	}
	stm << std::endl << "boxes:" << std::endl;
	for (auto& box : init.boxes)
	{
		stm << box;
	}
	return stm;
}

std::ostream& operator << (std::ostream& stm, const struct About_box& box)
{
	stm << "box: "  << box.box_name << "    disks:   ";
	for (auto& disk : *(box.disk_sym))
	{
		stm << "disk:  " << disk.first << "  sym:  " << disk.second << "  ";
	}
	stm << std::endl;
	return stm;
}





