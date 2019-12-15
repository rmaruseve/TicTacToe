#include "k_s_definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

int checkInput(void) {
	int validInput = 0, position;
	while (!validInput) {
		printf("Insert cordinates from 1 to 9 or %d for exit:  \n position: ", endMsg);
		scanf("%d", &position);
		if (position < 1 || position > 9) {
			if (position == endMsg) {
				validInput = 1;
			}
			else {
				printf("Invalid input. ");
			}
		}
		else {
			validInput = 1;
		}
	}
	return position;
}

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

	if (connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
		printError("Error - connect.");
	}

	//int buffer[ARRAY_LENGTH] = {0};
	char serverBuffer[BUFFER_LENGTH + 1];
	serverBuffer[BUFFER_LENGTH] = '\0';
	int koniec = 0, validMove, position;

	printf("Server connection has been established.\n");
	printf("This is the game of tic tac toe.\n");
	read(sock, serverBuffer, BUFFER_LENGTH);
	printf("Server has sent the following data: \n %s \n ", serverBuffer);

	while (!koniec) {
		printf("\nwe are starting new game....\n");
		position = checkInput();
		write(sock, &position, ARRAY_LENGTH);

		if (position != endMsg)
		{
			validMove = 0;
			while (!validMove) {
				
				//reading data from socket <unistd.h>
				read(sock, serverBuffer, BUFFER_LENGTH);
				if (strcmp(serverBuffer, errorMsg) == 0)
				{
					printf("%s", serverBuffer);

					position = checkInput();
					write(sock, &position, ARRAY_LENGTH);
				}
				else {
					validMove = 1;
				}
			}
			printf("1st Read...\nServer has sent the following data: \n %s\n", serverBuffer);

			

			read(sock, serverBuffer, BUFFER_LENGTH);
			printf("2nd Read...\nServer has sent the following data: \n %s\n", serverBuffer);
			koniec = 0;
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