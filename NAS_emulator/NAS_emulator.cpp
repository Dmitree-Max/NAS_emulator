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
#include <thread>
#include <vector>
#include "Requesthandler.h"



int main(int argc, char *argv[]){
    std::string a = "0101234ffffe";
    char ar[14];
    char code[7];
    char dst[14];
    string_into_array(&a, ar, 14);
    for(int i = 0; i < 14; i++)
    	printf("%c", ar[i]);
    printf("\n");
    code_signal(ar, code, 14);
    decode_signal(code, dst, 7);
    for(int i = 0; i < 14; i++)
    	printf("%c", dst[i]);
	/*
	int socker_descriptor = 0, connfd = 0;
	struct sockaddr_in serv_addr;

	std::vector<std::thread> connections;

	socker_descriptor = socket(AF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, '0', sizeof(serv_addr));



	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5060);


    bind(socker_descriptor, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(socker_descriptor, 10);

	printf("server started\n");
	int id = 0;
    while(true)
    {
    	connfd = accept(socker_descriptor, (struct sockaddr*)NULL, NULL);
    	connections.push_back(std::thread(handle_request, connfd, id));
    	id++;
    }
    for (auto &thr : connections)
    {
    	thr.join();
    }
    */

	return 0;
}
