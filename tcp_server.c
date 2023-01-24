#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
/*
IMPLEMENT:
C server should except an argument for the ip address
Fix the print statements not working
*/

//removes letters from a string
void parseInts(FILE* fp,char* string){
	char *p = string;	
	while(*p) {
		//checks if the current character is a digit
		if(isdigit(*p) || ( (*p=='-' ||*p=='+') && isdigit(*(p+1)) )) {
			long val = strtol(p, &p, 10);
			fprintf(fp, "%ld", val);
		}
		//else go to next char
		else {
			p++;
		}
	}
}
//removes digits from a string
char* parseChars(char* input) {
	char* dest = input;
	char* src = input;
	
	while(*src) {
	 //skips iteration if it detects a digit
	 if(isdigit(*src)) {src++; continue;}
	 *dest++ = *src++;	
	}
	*dest = '\0';
	return input;
}
int main() {
	 char server_message[256]  = "reached the server";
	 //get text file and open it
	 char *filename = "log.txt";
         //create socket
	 int netsocket;
	 netsocket = socket(AF_INET, SOCK_STREAM, 0);
	 
	//define addr struct
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;
	
	//bind
	bind(netsocket, (struct sockaddr*) &server_address,
        sizeof(server_address));
	//listen
	listen(netsocket, 5);
	while(1){
	int client_socket;
	client_socket = accept(netsocket,NULL,NULL);
	//checks if the text file opened correctly
	FILE *fp = fopen(filename, "a+");
	if(fp == NULL) {
	 printf("Error opening the text file: %s",filename);
	 return -1;
	}
	//receive data/name from client
	char client_message[256], *p = client_message, *p2 = client_message, *dest = client_message;
	send(client_socket, server_message, sizeof(server_message),0);
	recv(client_socket, &client_message, sizeof(client_message),0);
	  close(client_socket);
	//write data received from client to text file
	//checks if message  is the authorized length
	if(strlen(client_message) == 17 && strstr(client_message, "@") == NULL) {
		//prints client message to log.txt
		fprintf(fp, "%s\n" , client_message);
		fprintf(fp, " ints:\n");
		parseInts(fp,client_message);
		fprintf(fp,"\n chars:\n");
		fprintf(fp,"%s\n",parseChars(client_message));
		}
	else if (strstr(client_message, "@") != NULL) {
		time_t now = time(&now);
		char buffer[256];
		struct tm *ptm = gmtime(&now);
	
		strftime(buffer,80,"%x - %I:%m%p", ptm);
		
		
		//prints client message to log.txt
		fprintf(fp, "%s", client_message);
		fprintf(fp, " %s\n ", buffer);	
	}
	else {
		printf("unauthorized message\n");
	}
	//closing text file and socket
	fclose(fp);	
	}
         return 0;
	
}
