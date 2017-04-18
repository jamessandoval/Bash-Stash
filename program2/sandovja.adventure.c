/**********************
*
* James Sandoval
* CS 344 - Project#2
* sandovja.adventure.c
* "Adventure Interface"
***********************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

char pathTaken[20][50];
int stepCounter = 0;
int gameOver = 0;

// Structure to maintain room iterations
struct room{

  char connections[20][50];
  int lCounter;

};


// function to print time and output to file currenttime.txt
void printTime(void){
  
   char buffer[100];
   time_t currentTime;
   struct tm *loctime;
   FILE *fp;

   // Get current time from system
   currentTime = time (NULL);

   // convert current -> Local
   loctime = localtime (&currentTime);
   printf("%s", asctime (loctime));

   // write the time to the file specified:
   fp = fopen("currentTime.txt", "a+");
   fprintf(fp, "%s", asctime (loctime));
   fclose(fp);

}

// This function handles the file access portion and returns room data to main
struct room gamePlay(char* path){

  struct room roomInstance;

  char roomName[100];
  char roomType[100];
  char locations[100];
 
  FILE *fd1 = fopen(path, "r");
  int lines = 0;
  char *line2 = NULL;
  size_t len = 0;
  ssize_t read = 0;
  int counter = 0;
  // Counts the number of links
  roomInstance.lCounter = 0;
  // Counts the number of steps made
  stepCounter++;
  while ((read = getline(&line2, &len, fd1)) != -1) {
        ++counter;
        if(counter == 1){
          line2 +=11;
          strcpy (roomName,line2);
          printf("\nCURRENT LOCATION: %s", roomName);
          //counter++; 
        }else if(strncmp(line2, "CONN", 4) == 0){
           line2 +=14;
           strcpy (locations,line2);
           printf("POSSIBLE CONNECTION: %s", locations);
           // lcounter actually acccumulates here as it adds connections
           strcpy(roomInstance.connections[roomInstance.lCounter++], locations);
        }else{
           line2 +=11;
           strcpy (roomType,line2);
           if(strncmp(roomType, "END", 3) == 0){
              int gameOver = 1;
              printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
              printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS: \n", stepCounter);
                puts("Dungeon");
              for(int i=1; i <= stepCounter; i++){
                puts(pathTaken[i]);
              }
              exit(1);
           }
        }
    }
    fclose(fd1);
return roomInstance;
}


int main(void){
  char originalPath[] = "sandovja.rooms/Dungeon";
  char path2[100]= "sandovja.rooms/";
  char time[4] = "time";
  char comparison[20];
  struct room roomInstance;
  struct stat fileStat;

  if(stat("sandovja.rooms/", &fileStat) < 0){
    return 1;
  }

    // Enter the first room
   roomInstance = gamePlay(originalPath);
   int match = 0;

// Loop to accept user input until ending room is found
while(gameOver == 0){

  match = 0;

   // Loop to accept user input until valid input is found
   while(match == 0 || match == 2){

    printf("\nWHERE TO? > ");
    if (fgets (comparison, 20 , stdin) != NULL )
      if(strncmp(comparison, time, 4) == 0){
             match = 2;
             printTime();
      }    

    // remove \n characters placed by stdin
    strtok(comparison, "\n");
    strtok(time, "\n");

    // loop through all connections found in room returned from gamePlay
     for(int i = 0; i < roomInstance.lCounter; i++){
      strtok(roomInstance.connections[i], "\n");
          if(strcmp(comparison, roomInstance.connections[i]) == 0) {
             printf("Location Found %s\n", roomInstance.connections[i]);
             match = 1;
             strcat(path2, comparison);
             strcpy(pathTaken[stepCounter], comparison); 
             roomInstance = gamePlay(path2);
          }
     }
     // If a valid match is not found...continue
     if(match == 0){
        printf("That\'s not a valid location. try again.\n");
     }
  }
}

	return 0;
}
