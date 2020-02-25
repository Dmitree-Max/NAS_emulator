
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <iostream>
#include <bits/stdc++.h>
#include <string.h>


#include "socket_interactions.h"
#define COMMAND_LENGTH 26  //26 bytes = 52 symbols in 16's CS





void string_into_array(std::string* note, char* buffer, int buffer_length) {
	memset(buffer, '0', buffer_length);
	for (int i = 0; i < buffer_length; i++) {
		if (i < (*note).length()) {
			buffer[i] = (*note)[i];
		} else {
			buffer[i] = '0';
		}
	}

}

int main(int argc, char *argv[]) {
	printf("Client started\n");
	int socket_discriptor = 0;
	printf("\n");

	printf("%.s", argv[0]);

	if (argc != 2) {
		printf("\n Wrong number of arguments \n");
		return 1;
	}

	struct sockaddr_in serv_addr;

	memset(&serv_addr, '0', sizeof(serv_addr));



	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5060);

	if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
		printf("\n inet_pton error occurred\n");
		return 1;
	}

	std::string input;
	std::string command;
	while (1) {
		if ((socket_discriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("\n Error : Could not create socket \n");
			return 1;
		}
		if (connect(socket_discriptor, (struct sockaddr *) &serv_addr, sizeof(serv_addr))
				< 0) {
			printf("\n Error: Connect Failed \n");
			return 1;
		}

		std::cin >> command;
		int input_int = stoi(command);

		switch(input_int)
		{
		// device 1
			case 4:
				input = "00012900000000000000000000000000FFFF0004000000000000";
				break;
			case 5:
				input = "0001290000000000000000000000000000010005000000000000";
				std::string src, dst;
				std::string src_str, dst_str;
				std::cout << "source sym: " << std::endl;
				std::cin >> src;
				src_str = expand_to_byte(stoi(src));
				std::cout << "dist sym: " << std::endl;
				std::cin >> dst;
				dst_str = expand_to_byte(stoi(dst));
				input += src_str;
				input += dst_str;
				std::cout << "input:  " << input << std::endl;
				break;
		}


		int string_length = input.length();
		if(string_length < COMMAND_LENGTH)
		{
			std::cout << "string shorter then command";
			continue;
		}
		char temp_buff[string_length * 2];
		char sendBuff[string_length];
		memset(sendBuff, '0', sizeof(sendBuff));
		char answer_buffer[COMMAND_LENGTH];
		string_into_array(&input, temp_buff, 2 * string_length);
		code_signal(temp_buff, sendBuff, 2 * string_length);

		write(socket_discriptor, sendBuff, string_length);

		handle_answer(socket_discriptor, input_int);

		close(socket_discriptor);
	}

	return 0;
}





