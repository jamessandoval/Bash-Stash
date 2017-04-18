/**********************
*
* James Sandoval
* CS 344 - Project#2
* sandovja.buildrooms.c
* Room builder interface
***********************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <math.h>


// Structure to hold room data

struct rooms
  {
    int id;
    char *name;
    int connection[6];
    char *roomType;

  };

// Randomization function
static int randInt(int n) {
  int limit = RAND_MAX - RAND_MAX % n;
  int rnd;
 
  do {
     rnd = rand();
  } 
   
  while (rnd >= limit);
	return rnd % n;
}

// shuffle a given array
void shuffle(int *array, int n) {
    int i, j, tmp;
 
    for (i = n - 1; i > 0; i--) {
        j = randInt(i + 1);
        tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
   }
}

// convert Itoa function
char* itoa(int val, int base){
	
		static char buf[32] = {0};
	
		int i = 30;
	
		for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
		return &buf[i+1];
	
	}
// Write each room to a file with random details
void writerooms(struct rooms *roomArray){

	  const char* room[] = {"Dungeon", 
	  "Dragonroom",
	  "Gaurdtower", 
	  "Kitchen",
	  "Chapel", 
	  "Greathall",
	  "Gatehouse",
	  "Bedroom",
	  "Storeroom",
	  "Grounds"};

	  struct stat st = {0};

	  	// example of directory name smithj.rooms.19903
	  	char path1[100] = "sandovja.rooms/";
	  	char path2[100];
	  	// get process id
	  	char* alphapid = itoa(getpid(), 10);
	  	puts(alphapid);

		if (stat(path1, &st) == -1) {
       	    mkdir(path1, 0700);
         }

		      
		for(int i = 0; i < 10; i++){

			strcpy(path2, path1);
        	char *filename = roomArray[i].name;
        	strcat(path2, filename);
        	//strcat(path, );

        	puts(path2);
			
			FILE *f1 = fopen(path2, "w+");
			assert(f1);

			fprintf(f1, "ROOM NAME: %s\n", roomArray[i].name);

			// print connections 3 - 6 ...
			for(int j = 0; j < 6; j++){
				if(roomArray[i].connection[j] != -1){
				fprintf(f1, "CONNECTION %d: ", j+1);	
				fprintf(f1, "%s\n",room[roomArray[i].connection[j]]); 
				}
			}
			fprintf(f1, "ROOM TYPE: %s", roomArray[i].roomType);
			int fclose( FILE *fp );
		}

}

// print the room details here
void printrooms(struct rooms *roomArray){

		for(int i = 0; i < 10; i++){
			printf("Name:%s\n", roomArray[i].name);
			// print connections 3 - 6 ...
			for(int j = 0; j < 6; j++){
				printf("connection:%d\n", roomArray[i].connection[j]);	
			}
			printf("room type: TBD");
		}

}


// pass a singular room structure, returns 0 if successful, 1 if not.
// This function ultimately creates connections based on random assignments
int createConnection(struct rooms *roomArray, int idx){

    // new connection
	int link = randInt(10);
	if (link == roomArray[idx].id){
		return 1;
	}
		
	// check to see if this connection already exists, if it does, return 1, else add it
	int i = 0;
	while (i != 6){
		// if index is null, replace with the link
		if(roomArray[idx].connection[i] == -1){
			roomArray[idx].connection[i] = link;
			// link added, break the loop and proceed to secondary link
			break;
			
		}
		else if(roomArray[idx].connection[i] == link){
			return 1;
		}
		
		i++;
	}
	// Scan through all other rooms to add the link to adjacent room
	for(int i = 1; i < 10; ++i){
		// If the link is equal to id, we've found the room, so add the connection.
		if(roomArray[i].id == link){
			// loop through all connections to make sure no duplicates
			for(int j = 0; j < 6; j++){
				// if the connection is available, add it!
				if(roomArray[i].connection[j] == -1 && roomArray[i].id != link){
					roomArray[i].connection[j] = link;
					// return 0 if the link was created.
					return 0;
				}
				// link already exist
				else if(roomArray[i].connection[j] == link ){
					return 0;

				}
			}
		}
	}
	// simply to avoid warning from compiler:
	return 0;
}

// Add each connection to room
void addConnections(struct rooms *roomArray, int idx){
		int k = randInt(4); // rand seed 
		int linkCheck = 0;
		int counter = 0;
		int i = 0;

		if(k == 1){
			// room connections = 3
			// puts("3 connections");
			// pass the whole array, and pass the room so we don't have to find it again.
			
			while(counter != 3){
					linkCheck = createConnection(roomArray, idx);
					//printf("The output is: %d", linkCheck);
					if (linkCheck == 0){
							counter++;
					}
			}
		}
		else if(k == 2){
			// room connections = 4
			// puts("4 connections");
			while(counter != 4){
					linkCheck = createConnection(roomArray, idx);
					//printf("The output is: %d", linkCheck);
					if (linkCheck == 0){
							counter++;
					}
			}

		}else if(k == 3){
			// room connections = 5
			// puts("5 connections");
			while(counter != 5){
					linkCheck = createConnection(roomArray, idx);
					//printf("The output is: %d", linkCheck);
					if (linkCheck == 0){	
							counter++;
					}
			}

		}else{
			// room connections = 6
			// puts("6 connections");
			while(counter != 6){
					linkCheck = createConnection(roomArray, idx);
					if (linkCheck == 0){
							counter++;
					}
			}
		}
}

int main(void){

	// Initialize the rand seed
	srand(time(NULL));
	char *roomName, *connection1, *connection2, *connection3, *roomtype;
	char *roomDirectory = ("sandovja.rooms");
    mkdir(roomDirectory, 0700);

    // room begin = dungeon
    // room mid = chapel
    // room end =  gatehouse

    int A[10]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    shuffle(A, 10);

   	// define an array of rooms
   	struct rooms roomArray[10];

	for(int i = 0; i < 10; i++){
		roomArray[i].id = A[i];

		for (int k = 0; k < 6; k++){
		roomArray[i].connection[k] = -1;
		}
	}

	// Allocate Room structures

	for(int i = 0; i < 10; i++){	
		if(roomArray[i].id == 0){	
			roomArray[i].name = "Dungeon";
			roomArray[i].roomType= "START_ROOM";
			addConnections(roomArray, i);

		}
		else if(roomArray[i].id == 1){
			roomArray[i].name = "Dragonroom";
			roomArray[i].roomType= "AUX_ROOM";
			
			addConnections(roomArray, i);
			
		}
		else if(roomArray[i].id == 2){
			roomArray[i].name = "Gaurdtower";
			roomArray[i].roomType= "AUX_ROOM";

			addConnections(roomArray, i);
			
		}
		else if(roomArray[i].id == 3){
			roomArray[i].name = "Kitchen";
			roomArray[i].roomType= "AUX_ROOM";

			addConnections(roomArray, i);
			
		}
		else if(roomArray[i].id == 4){
			roomArray[i].name = "Chapel";
			roomArray[i].roomType= "AUX_ROOM";

			addConnections(roomArray, i);
			
		}
		else if(roomArray[i].id == 5){
			roomArray[i].name = "Greathall";
			roomArray[i].roomType= "MID_ROOM";
			addConnections(roomArray, i);
			
		}
		else if(roomArray[i].id == 6){
			roomArray[i].name = "Gatehouse";
			roomArray[i].roomType= "AUX_ROOM";

			addConnections(roomArray, i);
			
		}
		else if(roomArray[i].id == 7){
			roomArray[i].name = "Bedroom";
			roomArray[i].roomType= "AUX_ROOM";

			addConnections(roomArray, i);
			
		}
		else if(roomArray[i].id == 8){
			roomArray[i].name = "Storeroom";
			roomArray[i].roomType= "AUX_ROOM";

			addConnections(roomArray, i);

		}
		else if(roomArray[i].id == 9){
			roomArray[i].name = "Grounds";
			roomArray[i].roomType= "END_ROOM";
			addConnections(roomArray, i);

		}
}
	printf("\n\nRoom Attributes:\n");
	//printrooms(roomArray);
	writerooms(roomArray);

	return 0;
}
