#include "NAS_client.h"
#define COMMAND_LENGTH 52  //52 bytes = 26 symbols in ASCII



namespace nasclient{
	std::string send_command(std::string command, char* ip_adress, int port) {
		printf("Client started\n");
		int socket_discriptor = 0;
		printf("\n");

		struct sockaddr_in serv_addr;
		memset(&serv_addr, '0', sizeof(serv_addr));

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(port);

		if (inet_pton(AF_INET, ip_adress, &serv_addr.sin_addr) <= 0) {
			return std::string("\n inet_pton error occurred\n");
		}
		if ((socket_discriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			return std::string("\n Error : Could not create socket \n");
		}
		if (connect(socket_discriptor, (struct sockaddr *) &serv_addr, sizeof(serv_addr))
				< 0) {
			return std::string("\n Error: Connect Failed \n");
		}

		int string_length = command.length();
		if(string_length < COMMAND_LENGTH)
		{
			return std::string("string shorter then command");
		}
		if (string_length % 2 == 1){
			return std::string("odd command length");
		}
		char temp_buff[string_length];
		char sendBuff[string_length / 2];
		memset(sendBuff, '0', sizeof(sendBuff));
		string_into_array(&command, temp_buff, string_length);
		code_signal(temp_buff, sendBuff, string_length);
		write(socket_discriptor, sendBuff, string_length / 2);

		std::string result = handle_answer(socket_discriptor);

		close(socket_discriptor);
		return result;
	}
}


