#ifndef NAS_CLIENT_H_
#define NAS_CLIENT_H_


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


#include "socket_interactions.h"



namespace nasclient{
	std::string send_command(std::string command, char* ip_adress, int port);
}



#endif
