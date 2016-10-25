#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <time.h>


main() {
srand( time(NULL) );

//process id
int pid = getpid();

int size = (int)((ceil(log10(pid))+1)*sizeof(char)); //get size of process id
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
const char* roomNames[] = {"theGreatHall", "theAnteroom", "theMinotaur", "theHouse", "theFiveAndAHalfMinuteHallway", "theThreeAtticWhalestoeInstitution", "theAbyss", "waxAndJedsRoom", "mrMonstersCampsite", "theChildrensRoom"};

int roomNums[7] = {-1, -1, -1, -1, -1, -1, -1};
int k;
int o;
int randNum;
int bool = 1;
for(k = 0; k < 7; k++) {
	randNum = rand() % (9 + 1 - 0) + 0;	
	for (o = 0; o < 7; o++){
		if( roomNums[o] == randNum){
		bool = 0;
		k--;
		continue;
	
		}
	}		
	if(bool){
	roomNums[k] = randNum;
	printf("%d\n", randNum);
	}
	else bool = 1;
}


char buf[0x100];

char startRoom[50];
int rooms;
FILE * files[7];
for(rooms = 0; rooms < 7; rooms++){
snprintf(buf, sizeof(buf), "%s", roomNames[roomNums[rooms]]);
if(rooms == 4) strcpy(startRoom, buf); //get the startRoom name
printf("%s", buf);
files[rooms] = fopen(buf, "w");
fprintf(files[rooms], "ROOM NAME: %s\n", buf);//roomNames[roomNums[rooms]]);
}





int roomConnections[7][7];
memset(roomConnections, -1, sizeof(roomConnections));


int numConnections = 0;
int putMarks;

for (putMarks=0; putMarks < 7; putMarks++){
	numConnections = (rand() % (6 + 1 - 3)) + 3;
	roomConnections[putMarks][numConnections] = -2;
}

int x;
int l;
for (x = 0; x < 7; x++){
for (l = 0; l < 7; l++){
printf("  %d  ", roomConnections[x][l]);
}
printf("\n");
}

int connectionNum = 0;
int check2 = 0;
int i;
int j;
int p;
int count = 0;
for(i = 0; i < 7; i++){
//need to check to see if there are duplicate
	j = 0;
	
	while(roomConnections[i][j] != -1 && roomConnections[i][j] != -2){
	j++;
	}

	//if(roomConnections[i][j] == -2) continue;
	while( roomConnections[i][j] != -2 ){
		bool = 1;
		count++;
		if(count > 2000) break;
		connectionNum = (rand() % (6 + 1 - 0));
			
		while (connectionNum == i){
			connectionNum = (rand() % (6 + 1 - 0));
		}				
			
	//	printf("connection: %d, i: %d, j: %d\n", connectionNum, i, j);	

		for(p = 0; p < 6; p++) {	
			if (roomConnections[i][p] == connectionNum) {
				bool = 0;
				break;
			}
		}

		check2 = 0;		
		while(roomConnections[connectionNum][check2] != -1 && roomConnections[connectionNum][check2] != -2){
			check2++;
		}
	
		if (check2 == 6 || roomConnections[connectionNum][check2] == -2){
			bool = 0;
		}

		if(!bool){
			continue;
		}
		
		roomConnections[i][j] = connectionNum;
		
		roomConnections[connectionNum][check2] = i;
		j++;
}
}

printf("%d\n", count);
			


for (x = 0; x < 7; x++){
for (l = 0; l < 7; l++){
printf("  %d  ", roomConnections[x][l]);
}
printf("\n");
}
					

for (x = 0; x < 7; x++){
for (l = 0; l < 7; l++){
if(roomConnections[x][l] == -2 || roomConnections[x][l] == -1) {
	break;
}
else{
	snprintf(buf, sizeof(buf), "CONNECTION %d: %s", l+1, roomNames[roomConnections[x][l]]);
	fprintf(files[x], "%s\n", buf);  
}
}}

const char* names[3] = {"START_ROOM", "END_ROOM", "MID_ROOM"};
for (x=0; x < 7; x++) {
	if(x == 4)
	fprintf(files[x] , "ROOM TYPE: %s\n", names[0]);
	else if (x == 2) 
	fprintf(files[x], "ROOM TYPE: %s\n", names[1]);
	else
	fprintf(files[x], "ROOM TYPE: %s\n", names[2]);	

	fclose(files[x]);
}
//fclose(files);
char* visitedRooms[7] = {"x","x","x","x","x","x","x"};
char* currentConnections[6] = {"x","x","x","x","x","x"};

bool = 1;
//printf("%s", files[2]);
//snprintf(buf, sizeof(buf), "%s", roomNames[roomNums[4]]);
printf("start room is: %s\n", startRoom);




char filename[100];
strcpy(filename, startRoom);
//FILE *file = fopen (startRoom, "r");
i = 0;
printf("%s\n", filename);
while(bool == 1){
FILE* file = fopen(filename, "r");

if (file != NULL){
	char line[1000];
	while(fgets(line, sizeof line, file)!= NULL){
	printf(line);
	if(line[0] == 'C') {
			currentConnections[i] = strchr(line, ':')+2;// + strlen(line) - connLength;
			printf("%s\n", currentConnections[i]);
	}	
	else if(line[5] == 'T'){
			if(strchr(line, ':') + 2 == names[1]){
				printf("you've found the end room!");
				bool = 0;
			}
		}
	
}}
	else printf("errrrrrror no file");

printf("where you wanna go next?");
fgets(filename, 100, stdin);

}

return 0;
}
