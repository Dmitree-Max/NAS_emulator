#include "system_initialize.h"


extern std::list<struct Box_info> global_boxes;


void initialize_system(const char* filename)
{
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
	    return;
	}

	std::string text = std::string(memblock);
	delete(memblock);

	std::list<std::string>* parsed_by_sticks = parse_by_sticks(text);
	for (auto& box : *parsed_by_sticks)
	{
		std::list<int>* all_digits = find_numbers(box);

		Box* new_box = new Box(all_digits->front());
		all_digits->pop_front();

		std::list<struct Disk_info*>* temp = new std::list<struct Disk_info*>;
		while(all_digits->size() > 1)
		{
			struct Disk_info* cur = new struct Disk_info;
			cur->sym  = *(next(all_digits->begin()));
			std::cout << "sym: " << cur->sym << "\n";
			cur->disk = new Disk(all_digits->front());
			temp->push_back(cur);
			all_digits->pop_front();
			all_digits->pop_front();
		}
		new_box->set_disks(temp);
		struct Box_info new_box_info;
		new_box_info.box = new_box;
		new_box_info.box_name = new_box->get_number();
		global_boxes.push_back(new_box_info);
		delete(all_digits);
	}
	delete (parsed_by_sticks);
	return;
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







