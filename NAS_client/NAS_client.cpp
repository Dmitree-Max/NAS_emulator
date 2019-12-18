
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
#include "Coding.cpp"
#define COMMAND_LENGTH 22  //22 bytes = 44 symbols in 16's CS





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
	char temp_buff[COMMAND_LENGTH * 2];
	char sendBuff[COMMAND_LENGTH];
	char answer_buffer[COMMAND_LENGTH];
	printf("%.s", argv[0]);

	if (argc != 2) {
		printf("\n Wrong number of arguments \n");
		return 1;
	}

	struct sockaddr_in serv_addr;
	memset(sendBuff, '0', sizeof(sendBuff));
	memset(&serv_addr, '0', sizeof(serv_addr));



	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5060);

	if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
		printf("\n inet_pton error occurred\n");
		return 1;
	}

	std::string input;
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
		input = "";
		std::cin >> input;
		int string_length = input.length();
		string_into_array(&input, temp_buff, 2 * COMMAND_LENGTH);
		code_signal(temp_buff, sendBuff, 2 * COMMAND_LENGTH);

		write(socket_discriptor, sendBuff, COMMAND_LENGTH);

		memset(answer_buffer, '0', COMMAND_LENGTH);
		int error_code = 0;
		if ((error_code = read(socket_discriptor, answer_buffer, COMMAND_LENGTH)) > 0) {
			printf("answer: %s\n", answer_buffer);
		} else {
			if (error_code < 0) {
				printf("\n Read error \n");
				break;
			}
		}
		close(socket_discriptor);
	}

	return 0;
}
