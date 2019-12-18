//#include "TicTacToe_def.h"
#include "k_s_definitions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

char matrix[3][3];  /* the tic tac toe matrix */

void initMatrix(void)
{
	int i, j;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++) matrix[i][j] = ' ';
}

void getServerMove(void)
{
	int x, y;
	printf("Turn of server!\n");
	printf("Insert cordinates from 1 to 3:  \n x: ");
	scanf("%d", &x);
	printf(" y: ");
	scanf("%d", &y);
	x--; y--;
	
	if (matrix[x][y] != ' ') {
		printf("Invalid move, try again.\n");
		getServerMove();
	}
	else matrix[x][y] = 'O';

}

int getClientMove(int x, int y)
{
	printf("Client is playing...\n");
	x--; y--;
	

	if (matrix[x][y] != ' ') {
		return -1;
	}
	else matrix[x][y] = 'X';
	//checkifdraw(buffer);
	return 1;
}

int checkIfDraw()
{
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++)
			if (matrix[i][j] == ' ') break;
		if (matrix[i][j] == ' ') break;
	}

	if (i*j == 9) {
		return 2;
	}
	return 1;
}

void dispMatrixClient(char* buffer)
{
	strcpy(buffer, "\n");
	char line[100];
	int t;
	for (t = 0; t < 3; t++)
	{
		sprintf(line, " %c | %c | %c ", matrix[t][0], matrix[t][1], matrix[t][2]);
		strcat(buffer, line);

		if (t != 2)
			strcat(buffer, "\n---|---|---\n");
	}
	strcat(buffer, "\n");
}
void dispMatrix(void)
{
	int t;

	for (t = 0; t < 3; t++) {
		printf(" %c | %c | %c ", matrix[t][0],
			matrix[t][1], matrix[t][2]);
		if (t != 2) printf("\n---|---|---\n");
	}
	printf("\n\n");
}


char check(void)
{
	int i;

	for (i = 0; i < 3; i++)
		if (matrix[i][0] == matrix[i][1] &&
			matrix[i][0] == matrix[i][2]) return matrix[i][0];

	for (i = 0; i < 3; i++)
		if (matrix[0][i] == matrix[1][i] &&
			matrix[0][i] == matrix[2][i]) return matrix[0][i];

	if (matrix[0][0] == matrix[1][1] &&
		matrix[1][1] == matrix[2][2])
		return matrix[0][0];

	if (matrix[0][2] == matrix[1][1] &&
		matrix[1][1] == matrix[2][0])
		return matrix[0][2];

	if (checkIfDraw() == 2)
	{
		return 'D';
	}

	return ' ';
}
void readBuffer(int *x, int *y, char *buffer) {
	*x = atoi(buffer);
	char* str = strchr(buffer, ';');
	str++;
	*y = atoi(str);
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
	printf("This is the game of Tic Tac Toe.\n");
	initMatrix();
	//DO NOT CHANGE ORDER
	char done = ' ';
	char clientBuffer[BUFFER_LENGTH + 1];
	clientBuffer[BUFFER_LENGTH] = '\0';

	strncpy(clientBuffer, "-A--B-Ca\n", BUFFER_LENGTH);
	clientBuffer[BUFFER_LENGTH] = '\0';
	int koniec = 0, x, y, validMove;
	//int buffer[ARRAY_LENGTH] = {0};
	//^^DO NOT CHANGE ORDER^^

	while (!koniec) 
	{
		dispMatrix();
		dispMatrixClient(clientBuffer);
		write(clientSocket, clientBuffer, strlen(clientBuffer) + 1);

		while (done == ' ')
		{
			//reading data from socket <unistd.h>
			//client move
			read(clientSocket, clientBuffer, BUFFER_LENGTH);
			readBuffer(&x, &y, clientBuffer);

			printf("Client has sent the following data:\n%s\n", clientBuffer);
			if (atoi(clientBuffer) != endMsg) {
				validMove = 0;
				while (!validMove) {
					if (getClientMove(x, y) == -1)
					{
						printf("ERROR find %s", clientBuffer);
						strcpy(clientBuffer, "");
						strcpy(clientBuffer, errorMsg);
						write(clientSocket, clientBuffer, BUFFER_LENGTH);
						read(clientSocket, clientBuffer, BUFFER_LENGTH);
						readBuffer(&x, &y, clientBuffer);


						if (atoi(clientBuffer) == endMsg) {
							koniec = 1;
							validMove = 1;
						}
						else  if (getClientMove(x, y) != -1) {
							validMove = 1;
						}
					}
					else {
						validMove = 1;
					}
				}
				if ((done = check()) != ' '){
					/*strcat(clientBuffer, "BIGgggggggggTest");
					write(clientSocket, clientBuffer, strlen(clientBuffer) + 1);*/

					break;
				}
				//server turn
				dispMatrix();
				dispMatrixClient(clientBuffer);
				write(clientSocket, clientBuffer, strlen(clientBuffer) + 1);
				getServerMove();

				if ((done = check()) != ' ') {
					break;
				}

				dispMatrix();
				dispMatrixClient(clientBuffer);
				write(clientSocket, clientBuffer, strlen(clientBuffer) + 1);
			}
			else {
				koniec = 1;
				done = 'B';
			}
		}
		if (done == 'X') {
			done = ' ';
			dispMatrix();
			dispMatrixClient(clientBuffer);

			printf("Client won!\nClient can start new game.\n");
			strcat(clientBuffer, "Client won!\nYou can start new game.\n");
			write(clientSocket, clientBuffer, strlen(clientBuffer) + 1);
		
			initMatrix();
			strcpy(clientBuffer, "");
		}
		else if (done == 'O') {
			done = ' ';
			dispMatrix();
			dispMatrixClient(clientBuffer);

			printf("Server won!\nClient can start new game.\n");
			strcat(clientBuffer, "Server won!\nYou can start new game.\n");
			write(clientSocket, clientBuffer, strlen(clientBuffer) + 1);

			initMatrix();
			strcpy(clientBuffer, "");
		}
		else if (done == 'D') {
			done = ' ';
			dispMatrix();
			dispMatrixClient(clientBuffer);

			printf("DRAW!\nClient can start new game.\n");
			strcat(clientBuffer, "DRAW! \nYou can start new game.\n");
			write(clientSocket, clientBuffer, strlen(clientBuffer) + 1);
		
			initMatrix();
			strcpy(clientBuffer, "");
		}
	}
	printf("Client has terminated communication.\n");

	close(clientSocket);

	return (EXIT_SUCCESS);
}
