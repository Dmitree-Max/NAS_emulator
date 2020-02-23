/*
 * Socketinteractions.h
 *
 *  Created on: 23 февр. 2020 г.
 *      Author: dmitrii
 */

#ifndef SOCKETINTERACTIONS_H_
#define SOCKETINTERACTIONS_H_

class Socket_interactions {
public:
	Socket_interactions();
	virtual ~Socket_interactions();
	static std::string* get_additional_fields(int socket, int amount);
	static bool read_into_buffer(int socket, char* buffer, int symbol_ammount);
	static void fill_array_with_nules(char* array);
	static struct Request* get_command(int socket);
	static int write_command(int socket, std::string* command);
};

#endif /* SOCKETINTERACTIONS_H_ */
