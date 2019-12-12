#include <stdio.h>
#include <stdlib.h>
#include "TicTacToe_def.h"
//#include "k_s_definitions.h"

char matrix[3][3];  /* the tic tac toe matrix */



/*int main(void)
{
  char done;

  printf("This is the game of Tic Tac Toe.\n");


    done =  ' ';
    init_matrix();

    while(done == ' ')  
   {
    disp_matrix();
    get_player_move();
    disp_matrix();
    done = check(); 
   
    if(done!= ' ') break; 
   
    
    done = check();
   }
   
 

  if(done=='X')
	  printf("You won!\n");
  else 
	  printf("I won!!!!\n");
  
  disp_matrix(); 
  
  return 0;
}
*/

void init_matrix(void)
{
 
  int i, j;

  for(i=0; i<3; i++)
    for(j=0; j<3; j++) matrix[i][j] =  ' ';
}


void get_server_move(void)
{
  int x, y;
  printf("Turn of server!\n");
  printf("Enter X,Y coordinates for your move: ");
  scanf("%d%*c%d", &x, &y);

  x--; y--;
  if(matrix[x][y]!= ' '){
    printf("Invalid move, try again.\n");
    get_server_move_move();

  }
  else matrix[x][y] = 'O';
 
  //checkifdraw();
}

int getclientmove(int x,int y)
{

  x--; y--;

  if(matrix[x][y]!= ' '){
  return -1;
  }
  else matrix[x][y] = 'X';
 
 
  //checkifdraw(buffer);
  return 1;
}

int checkifdraw(char* buffer)
{
  int i, j;
  for(i=0; i<3; i++){
    for(j=0; j<3; j++)
      if(matrix[i][j]==' ') break;
    if(matrix[i][j]==' ') break;
  }

  if(i*j==9)  {
    printf("draw\n");
	strcpy(buffer,"draw\n",300);
	return -1;
  }
 return 1;
}

void disp_matrix_client(char* buffer)
{
	char line[100];
	int t;
	for(t=0; t<3; t++) 
	{
	sprintf(line,"%c | %c | %c ",matrix[t][0], matrix[t][1], matrix [t][2]);
	strcat(buffer,line);		
  
    if(t!=2)
		strcat(buffer,"\n---|---|---\n");
	}

	strcat(buffer,"\n");
}
void disp_matrix(void)
{
  int t;

  for(t=0; t<3; t++) {
    printf(" %c | %c | %c ",matrix[t][0],
            matrix[t][1], matrix [t][2]);
    if(t!=2) printf("\n---|---|---\n");
  }
  printf("\n");
}


char check(void)
{
  int i;

  for(i=0; i<3; i++)  
    if(matrix[i][0]==matrix[i][1] &&
       matrix[i][0]==matrix[i][2]) return matrix[i][0];

  for(i=0; i<3; i++)  
    if(matrix[0][i]==matrix[1][i] &&
       matrix[0][i]==matrix[2][i]) return matrix[0][i];

  
  if(matrix[0][0]==matrix[1][1] &&
     matrix[1][1]==matrix[2][2])
       return matrix[0][0];

  if(matrix[0][2]==matrix[1][1] &&
     matrix[1][1]==matrix[2][0])
       return matrix[0][2];

  return ' ';
} 