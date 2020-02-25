

#include "Socketinteractions.h"

Socket_interactions::Socket_interactions() {
	// TODO Auto-generated constructor stub

}

Socket_interactions::~Socket_interactions() {
	// TODO Auto-generated destructor stub
}




int Socket_interactions::write_command(int socket, std::string* command)
{
	int command_length = (*command).length();
	if (command_length % 2 == 1)
	{
		throw("command length is odd");
	}
	char buffer[command_length];
	char work_buffer[command_length / 2];

	string_into_array(command, buffer, command_length);
	code_signal(buffer, work_buffer, command_length);
	return write(socket, work_buffer, command_length / 2);
}



std::string* Socket_interactions::get_additional_fields(int socket, int amount)
{
	char additional_buffer[amount];
	char work_additional_buffer[2* amount];
	read_into_buffer(socket, additional_buffer, amount);
	decode_signal(additional_buffer, work_additional_buffer, amount);
	std::string* additional_fields = new std::string;
	char_array_into_string(work_additional_buffer, additional_fields, amount * 2);
	return additional_fields;
}


bool Socket_interactions::read_into_buffer(int socket, char* buffer, int buffer_size)
{
	memset(buffer, '0', buffer_size);

	int error = 0;
	socklen_t len = sizeof (error);
	int retval = getsockopt (socket, SOL_SOCKET, SO_ERROR, &error, &len);
	if (retval != 0) {
	    /* there was a problem getting the error code */
	    std::cout << "error getting socket error code: %s\n" << strerror(retval);
	    return true;
	}

	if (error != 0) {
	    /* socket has a non zero error status */
	    std::cout << "socket error: %s\n" << strerror(error);
		return true;
	}
	if ((error = read(socket, buffer, buffer_size)) > 0) {
	} else {
		if (error < 0) {
			printf("\n Read error \n");
			return true;
		}
	}
	return false;
}
