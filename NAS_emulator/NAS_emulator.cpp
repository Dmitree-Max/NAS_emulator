#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include "global_functions.h"
#include "global_variables_and_constants.h"
#include "Requesthandler.h"
#include "system_initialize.h"



int main(int argc, char *argv[]){

	initialize_system("auto_config.txt");


	int socker_descriptor = 0, connfd = 0;
	struct sockaddr_in serv_addr;

	std::vector<std::thread> connections;

	socker_descriptor = socket(AF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, '0', sizeof(serv_addr));



	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(WORKING_PORT);


    bind(socker_descriptor, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(socker_descriptor, 10);


	printf("server started\n");
	print_log(1, "server started\n");
	int id = 0;
    while(true)
    {
    	connfd = accept(socker_descriptor, (struct sockaddr*)NULL, NULL);
    	connections.push_back(std::thread(Request_handler::handle_request, connfd, id));
    	id++;
    }
    for (auto &thr : connections)
    {
    	thr.join();
    }


	return 0;
}
