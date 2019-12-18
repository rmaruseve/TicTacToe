#include "k_s_definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

void checkInput(char * buffer) {
	int validInput = 0;
	int x = 0, y = 0;
	while (!validInput) {
		printf("Insert cordinates from 1 to 3 or x = %d for exit:  \n x: ", endMsg);
		scanf("%d", &x);
		printf(" y: ");
		scanf("%d", &y);
		if ((x < 1 || x > 3) && (y < 1 || y > 3)) {
			if (x == endMsg) {
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
	sprintf(buffer, "%d;%d", x, y);
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
	char buffer[BUFFER_LENGTH + 1];
	buffer[BUFFER_LENGTH] = '\0';
	int koniec = 0, validMove;

	printf("Server connection has been established.\n");
	printf("This is the game of tic tac toe.\n");
	read(sock, buffer, BUFFER_LENGTH);
	printf("Server has sent the following data: \n %s \n ", buffer);

	while (!koniec) {
		printf("\nWe are starting new game....\n");
		checkInput(buffer);
		write(sock, buffer, BUFFER_LENGTH);

		if (atoi(buffer) != endMsg)
		{
			validMove = 0;
			while (!validMove) {
				
				//reading data from socket <unistd.h>
				read(sock, buffer, BUFFER_LENGTH);
				if (strcmp(buffer, errorMsg) == 0)
				{
					printf("%s", buffer);

					checkInput(buffer);
					write(sock, buffer, BUFFER_LENGTH);
				}
				else {
					validMove = 1;
				}
			}
			printf("1st Read...\nServer has sent the following data: \n %s\n", buffer);
			char* str = strchr(buffer, '!');
			if (str != NULL) {
				continue;
			}
			

			read(sock, buffer, BUFFER_LENGTH);
			printf("2nd Read...\nServer has sent the following data: \n %s\n", buffer);
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