
#include <iostream>
#include <string>
#include "../NAS_client/NAS_client.h"
#include "main_functions.h"
#include "Structure.h"



int main() {

	get_box_and_disks_configuration();
	get_other_disks();
	print_structure();
	structure->initialize_full_structure();




	//is device in box       0001290000000000000000000000000000000001000000000000
	//std::string comand1 = "0007290000000000000000000000000000000001000000000006";
	//std::string comand3   = "0001290000000000000000000000000000000001000000000000";
	// 0007 29000000 00000000 00000000 00000001 0003 0000 00000006
//	std::string comand2 = "037b290000000000000000000000000027100008000000000000";
	//std::string comand = make_simple_command(1, 1);
//	std::string answer;
//	answer = nasclient::send_command(comand2, "127.0.0.1", 5060);
//	std::cout << answer;
	delete(structure);
	return 0;
}
