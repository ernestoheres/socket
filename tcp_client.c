
#include  <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main (int argc, char *argv[]) {
	
	while(1) {
	//name from command line argument
	if(argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
	printf("please enter all arguments \n");
	return 0;
	}
	
	char client_message[256] = {0} ;
	strcat(client_message,argv[1]);
	//CREATE SOCKET
	int netsocket;
	netsocket = socket(AF_INET, SOCK_STREAM, 0);
	
	// set an address for the socket
	int port = atoi(argv[3]);
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	if(argv[2] == 0) {
		server_address.sin_addr.s_addr = INADDR_ANY;
	}
	else {
		server_address.sin_addr.s_addr = inet_addr(argv[2]);
	}
	int connection = connect(netsocket,(struct sockaddr *) &server_address,
	sizeof(server_address));
	if(connection ==  -1) {
		printf("There was an error making a connection \n\n");
	}

	
	//receive data
	char server_response[256];
	recv(netsocket, &server_response, sizeof(server_response),0);
	//send client name to server 
	send(netsocket, client_message, sizeof(client_message),0);
	//print data from server respnse
	printf("The server sent data: %s\n", server_response);
	
	//close socket
	close(netsocket);
	if(argv[4]) {
		break;	
	}
	sleep(60);
	}	
	return 0;
	
}

