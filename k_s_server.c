#include "k_s_definitions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

char* spracujData(char *data) {
    char *akt = data;
    while (*akt != '\0') {
        if (islower(*akt)) {
            *akt = toupper(*akt);
        }
        else if (isupper(*akt)) {
            *akt = tolower(*akt);
        } 
		akt++;		
    }
    return data;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printError("Sever has to be started with the following arguments: port.");
    }
    int port = atoi(argv[1]);
	if (port <= 0) {
		printError("Port has to be an integer number greater than 0.");
	}

    //creation of TCP socket <sys/socket.h>
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        printError("Error - socket.");        
    }
    
    //definition of server address <arpa/inet.h>
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;         //internet sockets
    serverAddress.sin_addr.s_addr = INADDR_ANY; //accepting connections from all over the Internet
    serverAddress.sin_port = htons(port);       //setting port
    
    //bind server address to socket <sys/socket.h>
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        printError("Error - bind.");
    }
    
    //server will accept new connections through the socket serverSocket <sys/socket.h>
    listen(serverSocket, 10);
    
    //server waits for client connection <sys/socket.h>
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);

    //closing passive socket <unistd.h>
    close(serverSocket);
    if (clientSocket < 0) {
        printError("Error - accept.");        
    }
	
    printf("Client has connected to the server.\n");
    char buffer[BUFFER_LENGTH + 1];
    buffer[BUFFER_LENGTH] = '\0';
    int koniec = 0;
    while (!koniec) {
        //reading data from socket <unistd.h>
		read(clientSocket, buffer, BUFFER_LENGTH);
        if (strcmp(buffer, endMsg) != 0) {
            printf("Client has sent the following data:\n%s\n", buffer);
            spracujData(buffer);
			//writing data to socket <unistd.h>
			write(clientSocket, buffer, strlen(buffer) + 1);
        }
        else {
            koniec = 1;
        }
    }
    printf("Client has terminated communication.\n");
    
    //closing client socket <unistd.h>
    close(clientSocket);
    
    return (EXIT_SUCCESS);
}