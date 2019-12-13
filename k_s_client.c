#include "k_s_definitions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>



int main(int argc, char *argv[]) {
    if (argc < 3) {
        printError("Client has to be started with the following arguments: address port.");
    }
    
    //getting server address and port <netdb.h>
    struct hostent *server = gethostbyname(argv[1]);
    if (server == NULL) {
        printError("Server does not exist.");
    }
    int port = atoi(argv[2]);
	if (port <= 0) {
		printError("Port has to be an integer number greater than 0.");
	}
    
    //creation of TCP socket <sys/socket.h>
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printError("Error - socket.");        
    }
    
    //definition of server address <arpa/inet.h>
    struct sockaddr_in serverAddress;
    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(port);

    if (connect(sock,(struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        printError("Error - connect.");        
    }
    
    printf("Server connection has been established.\n");
    char serverBuffer[BUFFER_LENGTH + 1];
    serverBuffer[BUFFER_LENGTH] = '\0';
    int buffer[ARRAY_LENGTH] = {0};
    int koniec = 0;
	read(sock, buffer, BUFFER_LENGTH);
    while (!koniec) {
        printf("Insert cordinates from 1 to 3:  \n x: ");
        scanf("%d", &buffer[0]);
        printf(" y: ");
        scanf("%d", &buffer[1]);
        printf("x %d\n", buffer[0]);

        // fgets(buffer, BUFFER_LENGTH, stdin);
        // char* pos = strchr(buffer, '\n');
        // if (pos != NULL) {
        //     *pos = '\0';
        // }
        //writing data to socket <unistd.h>
		
		write(sock, buffer, ARRAY_LENGTH);
        //if (strcmp(buffer, endMsg) != 0) {
        if(buffer[0] != 9){
            //reading data from socket <unistd.h>
			printf("Server has sent the following data: \n %s\n", serverBuffer);
			read(sock, buffer, BUFFER_LENGTH);
            
			
			read(sock, buffer, BUFFER_LENGTH);
			printf("Server has sent the following data: \n %s\n", serverBuffer);
			
        }
        else {
            koniec = 1;
        }
    }
    //closing socket <unistd.h>
    close(sock);
    printf("Server has terminated communication.\n");
    
    return (EXIT_SUCCESS);
}