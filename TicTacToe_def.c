#include <stdio.h>
#include <stdlib.h>
#include "TicTacToe_def.h"
//#include "k_s_definitions.h"

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

int getClientMove(int position)
{
	printf("Client is playing...\n");
	int x, y;
	//x--; y--;
	if (position == 1) {
		x = 0;
		y = 0;
	}
	else if (position == 1) {
		x = 0;
		y = 0;
	}
	else if (position == 2) {
		x = 0;
		y = 1;
	}
	else if (position == 3) {
		x = 0;
		y = 2;
	}
	else if (position == 4) {
		x = 1;
		y = 0;
	}
	else if (position == 5) {
		x = 1;
		y = 1;
	}
	else if (position == 6) {
		x = 1;
		y = 2;
	}
	else if (position == 7) {
		x = 2;
		y = 0;
	}
	else if (position == 8) {
		x = 2;
		y = 1;
	}
	else if (position == 9) {
		x = 2;
		y = 2;
	}
	else {
		return -1;
	}

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