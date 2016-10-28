#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <time.h>


main() {

//seed srand so that random numbers change every time
srand( time(NULL) );
//process id
int pid = getpid();
int size = 0;

//brute force method of getting size of pid
if(pid>=1000000) size = 7 ;
    else if(pid>=100000) size = 6;
	    else if(pid>=10000) size = 5;
		    else if(pid>=1000) size = 4;
			    else if(pid>=100) size = 3;
				    else if(pid>=10) size = 2;
					    else size = 1;



char numPID[size];

sprintf(numPID, "%d", pid); //change process id from string to a char

//combine name.rooms with the process id into a new char
char* fileDir = malloc(11 + strlen(numPID));	
strcpy(fileDir, "jib.rooms.");
strcat(fileDir, numPID);

//make a new directory
mkdir(fileDir, 0777);

//cd into this new directory
chdir(fileDir);

//ten room names
const char* roomNames[] = {"theGreatHall", "theAnteroom", "theMinotaur", "theHouse", "theFiveMinuteHallway", "theWhalestoeInstitution", "theAbyss", "waxAndJedsRoom", "mrMonstersCampsite", "theChildrensRoom"};

int roomNums[7] = {-1, -1, -1, -1, -1, -1, -1};
int k, o, randNum;
int bool = 1;

//get 7 random rooms
for(k = 0; k < 7; k++) {
	randNum = rand() % (9 + 1 - 0) + 0;	
	
	for (o = 0; o < 7; o++){
		if( roomNums[o] == randNum){
			bool = 0;
			k--;
			continue;	
		}
	}		
	
	if(bool) {
	roomNums[k] = randNum;
	}

	else bool = 1;
}

char buf[50];
char startRoom[50];
int rooms;

//file array will hold 7 different files for each room
FILE * files[7];

//create files and write the room name for each
for(rooms = 0; rooms < 7; rooms++){
	snprintf(buf, sizeof(buf), "%s", roomNames[roomNums[rooms]]);
if(rooms == 2) strcpy(startRoom, buf); //get the startRoom name

	buf[strcspn(buf, "\n")] = 0;
	
	files[rooms] = fopen(buf, "w"); //create a file for each room
	fprintf(files[rooms], "ROOM NAME: %s\n", buf); 
}


int roomConnections[7][7];  //array for roomConnections
memset(roomConnections, -1, sizeof(roomConnections));  //set all values in array to -1

int numConnections = 0;
int putMarks;

//this loop sets the number of connections for each room by marking the next index with -2
for (putMarks=0; putMarks < 7; putMarks++){
	numConnections = (rand() % (6 + 1 - 3)) + 3;
	roomConnections[putMarks][numConnections] = -2;
}

int connectionNum = 0;
int check2 = 0;
int i, j, l, p, x;
int count = 0;

//this is the loop that creates room connections
for(i = 0; i < 7; i++){
//need to check to see if there are duplicate
	j = 0;
	while(roomConnections[i][j] != -1 && roomConnections[i][j] != -2){
	j++;
	}
	//while we haven't reached all connections
	while( roomConnections[i][j] != -2 ){
		bool = 1;
		count++;
		if(count > 2000) break;
		connectionNum = (rand() % (6 + 1 - 0));
			
		while (connectionNum == i){
			connectionNum = (rand() % (6 + 1 - 0));
		}				
			

		for(p = 0; p < 6; p++) {	
			if (roomConnections[i][p] == connectionNum) {
				bool = 0;
				break;
			}
		}

		check2 = 0;		
		//finding the first non taken spot in the array
		while(roomConnections[connectionNum][check2] != -1 && roomConnections[connectionNum][check2] != -2){
			check2++;
		}
		//if the first spot was at the end of the array or -2
		if (check2 == 6 || roomConnections[connectionNum][check2] == -2){
			bool = 0;
		}

		if(!bool){
			continue;
		}
		//otherwise we're all good, set the respective connections
		roomConnections[i][j] = connectionNum;
		
		roomConnections[connectionNum][check2] = i;
		j++;
}
}
					
//open files and write connections to them
for (x = 0; x < 7; x++){
for (l = 0; l < 7; l++){
if(roomConnections[x][l] == -2 || roomConnections[x][l] == -1) {
	break;
}
else{
	snprintf(buf, sizeof(buf), "CONNECTION %d: %s", l+1, roomNames[roomNums[roomConnections[x][l]]]);
	fprintf(files[x], "%s\n", buf);  
}
}}

const char* names[3] = {"START_ROOM", "END_ROOM", "MID_ROOM"};

//write room types to the files
for (x=0; x < 7; x++) {
	if(x == 2)
	fprintf(files[x] , "ROOM TYPE: %s\n", names[0]);
	else if (x == 5) 
	fprintf(files[x], "ROOM TYPE: %s\n", names[1]);
	else
	fprintf(files[x], "ROOM TYPE: %s\n", names[2]);	

	fclose(files[x]);
}

//holds max of 20 visited rooms
char visitedRooms[20][30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
char currentConnections[7][30] = {0, 0, 0, 0, 0, 0, 0};

int visitedNumber = 0;
bool = 1;


char filename[100];
strcpy(filename, startRoom);
//while the end room hasn't been found
while(bool == 1){
//get the filename and open it
filename[strcspn(filename, "\n")] = 0;
FILE *file = fopen(filename, "r");

if (file != NULL){
	strcpy(visitedRooms[visitedNumber], filename);	
	visitedNumber++;
	i = 0;
	char line[1000];
	printf("CURRENT LOCATION: %s\n", filename);

	//reset the currentConnections array for the next location
	for(x = 0; x < 7; x++){
		strcpy(currentConnections[x],"");
	}
	while(fgets(line, sizeof line, file) != NULL){
	
	if(line[5] == 'T'){	//if we are on the ROOM TYPE line
		if(line[11] == 'E'){
			printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
			printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", visitedNumber);								for (l = 0; l < visitedNumber; l++){
				printf("%s\n", visitedRooms[l]);
				}
				bool = 0;
			}
		}

	else if(line[0] == 'C'){ //if we are on the CONNECTIONS line
		line[strcspn(line, "\n")] = 0;
		strcpy(currentConnections[i], strchr(line, ':')+2);
		i++;	
	}
	
	
}}
	else printf("\nHUH? I DON'T UNDERSTAND THAT NAME. TRY AGAIN.\n");


if(bool==1){

count = 0;
while(count == 0){

printf("POSSIBLE CONNECTIONS: ");
//print all connections
for(i = 0; i < 6; i++) {
	if(strcmp(currentConnections[i], "")==0) break;
	else if(strcmp(currentConnections[i+1],"")!=0) printf("%s, ", currentConnections[i]); 
	else printf("%s.\n", currentConnections[i]);
}

printf("WHERE TO? >");
fgets(filename, 100, stdin);

filename[strcspn(filename, "\n")] = 0;

for(i = 0; i < 6; i++){
	if(strcmp(currentConnections[i], filename) == 0) count = 1;
}

if(count == 0) printf("\nHUH? I DON'T UNDERSTAND THAT NAME. TRY AGAIN.\n");
}
}
//close file if it exists
if (file != NULL) fclose(file);
}

return 0;
}
