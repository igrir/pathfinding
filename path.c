/* Path finding
 *
 * written by Giri Prahasta Putra
 * website: http://prahasta.com
 * twitter: http://twitter.com/igrir
 *
 *
 * Universitas Pendidikan Indonesia
 * http://cs.upi.edu
 */

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

/*	CREATE THE DOUBLE LIST  */

//create the coordinate holder
typedef struct{
	//the coordinate consists of x, y, and the number of iteration
	//for example (2,3,0)
	int c[4];
}coordinate;

//the address for next element
typedef struct elmt * address;

//create the element of list
typedef struct elmt
{
	coordinate cord;
	address prev;
	address next;
}element;

//create the list
typedef struct
{
	//the list itself hold the first and the last element
	element * first;
	element * tail;
}list;

void createList(list * L){
	(*L).first = NULL;
	(*L).tail  = NULL;
}

void addFirst(list *L, int x, int y, int i){
	element * new = (element *) malloc(sizeof(element));

	//add the data
	new->cord.c[0] = x;
	new->cord.c[1] = y;
	new->cord.c[2] = i;

	if ((*L).first == NULL) {
		(*L).first = new;
		(*L).tail  = new;
		new->next  = NULL;
		new->prev  = NULL;
	}else{
		new->next = (*L).first;
		(*L).first->prev = new;
		(*L).first = new;
		new->prev = NULL;
	}
}

void addLast(list *L, int x, int y, int i){

	if ((*L).first == NULL) {
		addFirst(L, x, y, i);
	}else{
		element * new = (element *) malloc(sizeof(element));

		//add the data
		new->cord.c[0] = x;
		new->cord.c[1] = y;
		new->cord.c[2] = i;

		//add the new element
		(*L).tail->next = new;
		new->prev = (*L).tail;
		(*L).tail = new;
		new->next = NULL;
	}

}

void delNow(list *L, element * now){
	//if the last element
	if (now == (*L).first && now == (*L).tail) {
		(*L).first = NULL;
		(*L).tail  = NULL;
		free(now);
	//if the first element
	}else if (now == (*L).first) {
		(*L).first = now->next;
		(*L).first->prev = NULL;
		now->next = NULL;

		free(now);
	//if the tail element
	}else if (now == (*L).tail) {
		(*L).tail  = now->prev;
		(*L).tail->next = NULL;
		now->prev = NULL;

		free(now);
	//if at the middle
	}else{
		now->prev->next = now->next;
		now->next->prev = now->prev;
		now->next = NULL;
		now->prev = NULL;
		free(now);
	}
}


void printElements(list L){
	element * now = L.first;

	while (now != NULL) {
		printf("x: %d, y: %d, i: %d\n",now->cord.c[0], now->cord.c[1], now->cord.c[2]);
		now = now->next;
	}
}

//this function return 1 if the path is in the list
int checkPathExist(list L, int x, int y){
	element * now = L.first;
	int exist = 0;

	while (now != NULL && exist == 0) {
		if (now->cord.c[0] == x && now->cord.c[1] == y) {
			exist = 1;
		}
		now = now->next;
	}

	return exist;
}

//return the number of iteration hold by the node
int returnIteration(list L, int x, int y){
	element * now = L.first;
	int iteration = 0;
	int found = 0;
	while (now != NULL && found == 0) {
		if (now->cord.c[0] == x && now->cord.c[1] == y) {
			found = 1;
			iteration = now->cord.c[2];
		}
		now = now->next;
	}

	return iteration;
}

int main()	{

	list L;
	//initialize the list for the path
	createList(&L);

	//intialize the map

	//0 = free
	//1 = wall
	//2 = start
	//3 = end
	int arr[11][11] = {	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						{1, 0, 0, 0, 1, 1, 0, 1, 0, 1},
						{1, 0, 1, 0, 0, 1, 0, 0, 0, 1},
						{1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
						{1, 0, 1, 0, 0, 1, 0, 0, 0, 1},
						{1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
						{1, 0, 0, 0, 0, 1, 0, 1, 0, 1},
						{1, 0, 1, 1, 0, 0, 0, 1, 0, 1},
						{1, 0, 1, 0, 0, 1, 0, 1, 0, 1},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
					  };

	
	//start
	int xstart = 1;
	int ystart = 3;

	//end
	int xend   = 8;
	int yend   = 8;

	//using arr[y][x] because arr[row][column]
	//set the start
	arr[ystart][xstart] = 2;
	//set the end
	arr[yend][xend] = 3;

	int i, j;

	for (i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			if (arr[i][j] == 9) {
				printf("* ");
			}else if (arr[i][j] == 0) {
				printf("  ");
			}else if (arr[i][j] == 2) {
				printf("S ");
			}else if (arr[i][j] == 3) {
				printf("0 ");
			}else{
				printf("%d ", arr[i][j]);	
			}
			
		}
		printf("\n");
	}

	//start from the end

	//add the first element to the list, the end
	//of the path
	addFirst(&L, xend, yend, 0);

	int found = 0;
	int cx = xend;
	int cy = yend;
	int ci = 0;

	element * now = L.first;

	//do the iteration until the first node found
	while (found == 0 && now != NULL) {

		

		//check the adjacent of four direction not wall

		int nowx = now->cord.c[0];
		int nowy = now->cord.c[1];
		int nowi = now->cord.c[2];

		//check the up
		if (nowx == xstart && nowy-1 == ystart) {
			found = 1;
			addLast(&L, nowx, nowy-1, nowi+1);
		}
		if (found == 0) {
			if (nowy-1 >= 0) {
				if (checkPathExist(L, nowx, nowy-1) == 0) {
					if (arr[nowy-1][nowx] != 1) {
						addLast(&L, nowx, nowy-1, nowi+1);
					}
				}
			}
		}
		
		
		//check the down
		if (nowx == xstart && nowy+1 == ystart) {
			found = 1;
			addLast(&L, nowx, nowy+1, nowi+1);
		}
		if (found == 0) {
			//in here check to 9 because the maximum of the map is 9
			if (nowy+1 <= 9) {
				if (checkPathExist(L, nowx, nowy+1) == 0) {
					if (arr[nowy+1][nowx] != 1) {
						addLast(&L, nowx, nowy+1, nowi+1);
					}
				}
			}
		}	

		//check the left
		if (nowx-1 == xstart && nowy == ystart) {
			found = 1;
			addLast(&L, nowx-1, nowy, nowi+1);
		}
		if (found == 0) {
			if (nowx-1 >= 0) {
				if (checkPathExist(L, nowx-1, nowy) == 0) {
					if (arr[nowy][nowx-1] != 1) {
						addLast(&L, nowx-1, nowy, nowi+1);
					}
				}
			}
		}

		//check the right
		if (nowx+1 == xstart && nowy == ystart) {
			found = 1;
			addLast(&L, nowx+1, nowy, nowi+1);
		}
		if (found == 0) {
			if (nowx+1 <= 9) {
				if (checkPathExist(L, nowx+1, nowy) == 0) {
					if (arr[nowy][nowx+1] != 1) {
						addLast(&L, nowx+1, nowy, nowi+1);
					}
				}
			}
		}

		now = now->next;
	}

	//do the next step if there was a solution
	if (found == 1) {


		/*********** FROM START TO END ***********/
		found = 0;

		//create the list that hold the track
		list L2;
		createList(&L2);

		addFirst(&L2, L.tail->cord.c[0], L.tail->cord.c[1], L.tail->cord.c[2]);

		//check from the start to the end to find the short path
		//from the built path and also remove the unnecessary path

		now = L.tail;

		int backX = L.tail->cord.c[0];
		int backY = L.tail->cord.c[1];
		int backI = L.tail->cord.c[2];

		while(found == 0){
			//the holder of the candidate for the 
			int candidateX;
			int candidateY;
			int candidateI;

			//check the adjacent from the last one

			//check the adjacent of four direction not wall and check it's the shortest iteration one
			
			//check up
			//check whether it's in the list
			if (backY-1 >= 0) {
				if (checkPathExist(L, backX, backY-1) == 1) {
					
					if (returnIteration(L, backX, backY-1) < backI) {
						candidateX = backX;
						candidateY = backY-1;
						candidateI = returnIteration(L, backX, backY-1);
					}

				}
			}

			//check down
			if (backY+1 <= 9) {
				if (checkPathExist(L, backX, backY+1) == 1) {
					if (returnIteration(L, backX, backY+1) < backI) {
						candidateX = backX;
						candidateY = backY+1;
						candidateI = returnIteration(L, backX, backY+1);
					}
				}
			}

			//check left
			if (backX-1 >= 0) {
				if (checkPathExist(L, backX-1, backY) == 1) {
					if (returnIteration(L, backX-1, backY) < backI) {
						candidateX = backX-1;
						candidateY = backY;
						candidateI = returnIteration(L, backX-1, backY);
					}
				}
			}

			//check right
			if (backX+1 <= 9) {
				if (checkPathExist(L, backX+1, backY) == 1) {
					if (returnIteration(L, backX+1, backY) < backI) {
						candidateX = backX+1;
						candidateY = backY;
						candidateI = returnIteration(L, backX+1, backY);

					}
				}
			}

			addLast(&L2, candidateX, candidateY, candidateI);

			backX = candidateX;
			backY = candidateY;
			backI = candidateI;

			//stop it when reach the end path
			if (backX == xend && backY == yend) {
				found = 1;
			}

		}



		//draw the path to the map

		element * draw = L2.first;

		while (draw != NULL) {
			if (draw != L2.first && draw != L2.tail) {
				arr[draw->cord.c[1]][draw->cord.c[0]] = 9;
			}
			
			draw = draw->next;
		}

		for (i=0;i<10;i++)
		{
			for(j=0;j<10;j++)
			{
				if (arr[i][j] == 9) {
					printf("* ");
				}else if (arr[i][j] == 0) {
					printf("  ");
				}else if (arr[i][j] == 2) {
					printf("S ");
				}else if (arr[i][j] == 3) {
					printf("0 ");
				}else{
					printf("%d ", arr[i][j]);	
				}
				
			}
			printf("\n");
		}

	}else{
		printf("There is no solution\n");
	}

	return 0;
}