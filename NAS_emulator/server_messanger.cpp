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
#include <stdlib.h>
#include <time.h>



void handle_request(int socket, int id, std::string* this_answers)
{
	bool exit = false;
	char buffer[1024];
	char answer_buffer[1024];
	int error_code = 0;
	while(!exit)
	{
		memset(buffer, '0', sizeof(buffer));
	  	std::string comand = "";
	    if ((error_code = read(socket, buffer, 1024)) > 0)
	    {
	    	for (int i = 0; buffer[i] != '\0'; i++)
	    	{

	    		comand += buffer[i];
	    		if (i == 1023)
	    		{
	   				if (buffer[i] == '0')
	 				{
	    				std::cout << "exited\n";
	  					exit = true;
	  					break;
	    			}
	    			else
	    			{
	    				printf("Wrong signal\n");
	    			}
	    		}
                        if (i == 1025)
				break;
	    	}
	    }
	    else
	    {
	    	if(error_code <= 0)
	    	{
	    		printf("\n Read error \n");
	   			break;
	 		}
	    }
	    if (exit)
			break;
	    std::cout << "I got:  " << comand << " from " << id << "\n";
	    int k = rand() % 13;
	    std::string answer = "You said me \"" + comand + "\"" + ", humorous! But i will tell you, that \"" + this_answers[k] + "\"\n";
		memset(answer_buffer, '0', sizeof(answer_buffer));
		for (int j = 0; j < answer.length() + 1; j++)
			answer_buffer[j] = answer[j];

		if (error_code = write(socket, answer_buffer, 1024) <= 0)
		{
			printf("Connection %i failure\n", id);
			break;
		}

	}
	close(socket);
	printf("Connection %i closed\n", id);
}


int main(int argc, char *argv[]){
	int socker_descriptor = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	srand (time(NULL));

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

	std::string answers[13] = {"Houston, we have a problem", "I'll be back", "It's alive! It's alive!", "I've got a feeling we're not in Kansas anymore",
								"There's no place like home", "I love the smell of napalm in the morning", "Frankly, my dear, I don't give a damn",
								"I feel the need... the need for speed", "I see dead people", "I'm gonna make him an offer he can't refuse",
								"I think this is the beginning of a beautiful friendship", "You can't handle the truth!", "MatMeh is the best"};
    while(true)
    {
    	connfd = accept(socker_descriptor, (struct sockaddr*)NULL, NULL);
    	printf("Connection %i is accepted\n", id);
    	connections.push_back(std::thread(handle_request, connfd, id, answers));
    	id++;
    }
    for (auto &thr : connections)
    {
    	thr.join();
    }

	return 0;
}
