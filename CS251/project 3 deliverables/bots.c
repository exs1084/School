#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <time.h>


typedef struct{
	int x;
	int y;
}loc;

typedef struct{//thread arguments
	loc location;
	int tID;
}parg;

int numBots=0, height=0, width=0;
char mode = 'B';
bool found = false;
loc foundLoc;


int **dists;//distance of each bot from each target location
int *locChoices;//the loc index that each bot should go to, in order
loc *locs;//target locations for surrounding
loc *botLocs;//current locations of bots
int *moves;//record of how many moves each bot has made
bool *complete;//if each bot is done surrounding

pthread_mutex_t mutex1;//for movement
pthread_barrier_t barrier1;//for all including parent thread
pthread_barrier_t barrier2;//only for bots

int str_to_int( char str_score[] ) {//method provided by a previous instructor

	int value = 0;
	for ( int j = 0; str_score[j] != '\0'; ++j ) {
		if ( isdigit( str_score[j] ) ) {
		value = value * 10 + (str_score[j] - '0');
		} else {
			if(str_score[j]=='.'){//it was a double, just stop.
				return value;
			}
			// was not a number, return -1 to show this
			return -1;
		}
	}
	return value;
}

void findLocs(){//find the target locations to surround the object
	printf("calculating target locations...\n");
	locs = (loc *)malloc(sizeof(loc) * numBots);
	dists = (int **)malloc(sizeof(int *) * numBots);
	int R=1, offset=0, locsFound=0;//R=ring
	while(true){
		locs[locsFound].x = foundLoc.x + offset, locs[locsFound].y = foundLoc.y + R;
		if(locs[locsFound].x>=0 && locs[locsFound].x<width && locs[locsFound].y>=0 && locs[locsFound].y<height){
			locsFound++;//only keep this location if its valid, else replace
		}
		if(locsFound>=numBots){
			break;
		}

		locs[locsFound].x = foundLoc.x + R, locs[locsFound].y = foundLoc.y - offset;
		if(locs[locsFound].x>=0 && locs[locsFound].x<width && locs[locsFound].y>=0 && locs[locsFound].y<height){
			locsFound++;//only keep this location if its valid, else replace
		}
		if(locsFound>=numBots){
			break;
		}

		locs[locsFound].x = foundLoc.x - offset, locs[locsFound].y = foundLoc.y - R;
		if(locs[locsFound].x>=0 && locs[locsFound].x<width && locs[locsFound].y>=0 && locs[locsFound].y<height){
			locsFound++;//only keep this location if its valid, else replace
		}
		if(locsFound>=numBots){
			break;
		}

		locs[locsFound].x = foundLoc.x - R, locs[locsFound].y = foundLoc.y + offset;
		if(locs[locsFound].x>=0 && locs[locsFound].x<width && locs[locsFound].y>=0 && locs[locsFound].y<height){
			locsFound++;//only keep this location if its valid, else replace
		}
		if(locsFound>=numBots){
			break;
		}

		if(offset>=R){//new ring
			R++;
			offset=0;
		}else{
			if(offset<=0){
				offset*=-1;
				offset++;
			}else{
				offset*=-1;
			}//new set in a ring
		}
	}

}

void findDist(int tID){//write this bots distances to each target location
	printf("bot %i calculating distances...\n", tID);
	dists[tID] = (int *)malloc(sizeof(int) * numBots);
	for(int i=0;i<numBots;i++){
		int dist=0;
		int botx=botLocs[tID].x;
		int locx=locs[i].x;
		int d = abs(botx - locx);
		dist+=d;
		dist+=abs(botLocs[tID].y - locs[i].y);
		dists[tID][i] = dist;//botLocs
	}
}

//run by tID 1. decides where all bots will go to.
void pickLocs(){
	//start with everyone goes to index=tID, loop through each column of a dist matrix
	//and check if swapping the locations will reduce the total distance
	printf("deciding everyones target locations...\n");
	locChoices = (int *)malloc(sizeof(int) * numBots);
	for(int i=0;i<numBots;i++){
		locChoices[i] = i;//initialize a default set
	}
	for(int i=0;i<numBots;i++){
		for(int j=0;j<numBots;j++){
			if(i!=j){
				int sum1, sum2;
				sum1 = dists[i][locChoices[i]]+dists[j][locChoices[j]];
				sum2 = dists[i][locChoices[j]]+dists[j][locChoices[i]];
				if(sum2<sum1){
					int temp = locChoices[i];
					locChoices[i] = locChoices[j];
					locChoices[j] = temp;
				}
			}
		}
	}
}

void move(int tID, int direction, bool surround){//0=up, 1=right, 2=down, 3=left
	pthread_mutex_lock(&mutex1);
	int nexX=botLocs[tID].x, nexY=botLocs[tID].y;
	if(surround){//surround func
		if(nexX==locs[locChoices[tID]].x && nexY==locs[locChoices[tID]].y ){//done moving?
			pthread_mutex_unlock(&mutex1);
			return;
		}
	}else{//searching func
		if(abs(botLocs[tID].x-foundLoc.x)+abs(botLocs[tID].y-foundLoc.y)==1){//found it?
			pthread_mutex_unlock(&mutex1);
			return;
		}
		if(found){//different bot found it?
			pthread_mutex_unlock(&mutex1);
			return;
		}
	}
	switch(direction%4){
		case 0:
			nexY++;break;
		case 1:
			nexX++;break;
		case 2:
			nexY--;break;
		case 3:
			nexX--;break;
	}
	for(int i=0;i<numBots;i++){
		if(botLocs[i].x==nexX && botLocs[i].y==nexY){//space occupied, go around
			pthread_mutex_unlock(&mutex1);
			
			move(tID, direction+3, surround);
			move(tID, direction, surround);
			move(tID, direction, surround);
			move(tID, direction+1, surround);
			return;
		}
	}
	if(nexX==foundLoc.x && nexY==foundLoc.y){//space occupied, go around
		pthread_mutex_unlock(&mutex1);
		move(tID, direction+3, surround);
		move(tID, direction, surround);
		move(tID, direction, surround);
		move(tID, direction+1, surround);
		return;
	}
	botLocs[tID].x = nexX, botLocs[tID].y = nexY;
	pthread_mutex_unlock(&mutex1);
	moves[tID]++;
	if(!surround){
		if(abs(botLocs[tID].x-foundLoc.x)+abs(botLocs[tID].y-foundLoc.y)==1){
			found=true;//static object found
		}
		if(found){
			pthread_mutex_unlock(&mutex1);
			return;
		}
	}
	pthread_barrier_wait(&barrier2);
	return;
}

void surround(int tID, int locIndex){
	//continually call move until target loc is reached
				printf("thread %i moving to loc %i\n", tID, locIndex);
	int targX = locs[locIndex].x, targY = locs[locIndex].y;
	while(botLocs[tID].x!=targX){
		if(targX>botLocs[tID].x){
			move(tID, 1, true);
		}else{
			move(tID, 3, true);
		}
	}
	while(botLocs[tID].y!=targY){
		if(targY>botLocs[tID].y){
			move(tID, 0, true);
		}else{
			move(tID, 2, true);
		}
	}
	sleep(0.7);
	complete[tID]=true;
printf("bot %i reached %i, %i after %i total moves\n", tID, botLocs[tID].x, botLocs[tID].y, moves[tID]);
	pthread_barrier_wait(&barrier2);
	bool allDone=true;
	while(true){
		for(int i=0;i<numBots;i++){
			if(complete[i]==false){allDone=false;}
		}
		if(allDone){
			break;
		}else{
			allDone=true;
			
		}
		pthread_barrier_wait(&barrier2);
	}
}

void search(int tID){
printf("bot %i begins search...\n", tID);
		if(abs(botLocs[tID].x-foundLoc.x)+abs(botLocs[tID].y-foundLoc.y)==1){
			found=true;//other bots will break at end of loop
			pthread_barrier_wait(&barrier2);
printf("bot %i found object without moving...\n", tID);
			return;//dont waste time moving to another cell, already in position
		}
	int targX = 0, targY = tID*2;
	int state=0;//0=preparing, 1=right, 2=down, 3=left
	pthread_barrier_wait(&barrier2);
	sleep(1);
	while(!found){
		while(botLocs[tID].y!=targY){
			if(targY>botLocs[tID].y){
				move(tID, 0, false);
			}else{
				move(tID, 2, false);
			}
			if(abs(botLocs[tID].x-foundLoc.x)+abs(botLocs[tID].y-foundLoc.y)==1){
				sleep(1);
				found=true;//other bots will break at end of loop
				pthread_barrier_wait(&barrier2);
				printf("bot %i exiting search after %i moves...\n", tID, moves[tID]);
				return;//dont waste time moving to another cell, already in position
			}
			pthread_barrier_wait(&barrier2);
			if(found){
				printf("bot %i exiting search after %i moves...\n", tID, moves[tID]);
				return;
			}
		}
		while(botLocs[tID].x!=targX){
			if(targX>botLocs[tID].x){
				move(tID, 1, false);
			}else{
				move(tID, 3, false);
			}
			if(abs(botLocs[tID].x-foundLoc.x)+abs(botLocs[tID].y-foundLoc.y)==1){
				sleep(1);
				found=true;//other bots will break at end of loop
				pthread_barrier_wait(&barrier2);
				printf("bot %i exiting search after %i moves...\n", tID, moves[tID]);
				return;//dont waste time moving to another cell, already in position
			}
			pthread_barrier_wait(&barrier2);
			if(found){
				printf("bot %i exiting search after %i moves...\n", tID, moves[tID]);
				return;
			}
		}
		switch(state){
			case 0:
				state=1;
				targX=width-1;
				break;
			case 1:
				state=2;
				targY+=numBots*2;
				if(targY>height){
					targY=0, targX=tID;
				}
				break;
			case 2:
				if(targX==0){
					state=1;
					targX=width-1;
				}else{
					state=3;
					targX=0;
				}
				break;
			case 3:
				state=2;
				targY+=numBots*2;
				if(targY>height){
					targY=0, targX=tID;
				}
				break;
		}
		
	}
printf("bot %i exiting search after %i moves...\n", tID, moves[tID]);
}

void *botFunc(void *in){
	int tID = *(int *)in;
	moves[tID]=0;
	complete[tID]=false;
	printf("creating bot %i...\n", tID);

	if(mode == 'B'){
		search(tID);//search
		pthread_barrier_wait(&barrier2);
	}
	if(tID==0){
		findLocs();
		for(int i=0;i<numBots;i++){
			printf("target location %i is %i, %i\n", i, locs[i].x, locs[i].y);
		}
		pthread_barrier_wait(&barrier2);
	}
	while(locs==NULL){//correct synch issues
		pthread_barrier_wait(&barrier2);
	}
	pthread_barrier_wait(&barrier2);
	findDist(tID);
	printf("bot %i distances: ", tID);
	for(int i=0;i<numBots;i++){
		printf("%i, ", dists[tID][i]);
	}
	printf("\n");
	pthread_barrier_wait(&barrier2);
	if(tID==0){
		bool distDone=true;
		while(true){//make sure all bots done with findDist..had some bugs
			for(int i=0;i<numBots;i++){
				if(dists[i]==NULL){
					distDone=false;
				}
			}
			if(distDone){
				break;
			}else{
				distDone=true;
			}
		}
		pickLocs();
		printf("location choices are: ");
		for(int i=0;i<numBots;i++){
			printf("%i ", locChoices[i]);
		}
		printf("\n");
		pthread_barrier_wait(&barrier2);
	}
	while(locChoices==NULL){//correct synch issues
		pthread_barrier_wait(&barrier2);
	}
	pthread_barrier_wait(&barrier2);
	surround(tID, locChoices[tID]);

						pthread_barrier_wait(&barrier1);//4
}

//the main program that interprets command line arguments and runs threads
//
//@param argc number of args
//@Param argv args
//@return failure or success
int main(int argc,char *argv[]){
	srand(time(NULL));
				printf("running bot program...\n");
	char input[10];
	printf("how wide is the board?\n");
	scanf("%s", input);
	width = str_to_int(input);
	printf("how tall is the board?\n");
	scanf("%s", input);
	height = str_to_int(input);
	printf("how many robots will be used?\n");
	scanf("%s", input);//read input
	numBots = str_to_int(input);
	printf("what mode will be used(A or B)?\n");
	scanf("%s", input);
	mode = input[0];

	foundLoc.x=rand()%width, foundLoc.y=rand()%height;
	printf("static object at %i, %i\n", foundLoc.x, foundLoc.y);
	pthread_barrier_init(&barrier1, NULL, numBots+ 1);
	pthread_barrier_init(&barrier2, NULL, numBots);
	pthread_t threads[numBots];
	int tID[numBots];
	moves = (int *)malloc(sizeof(int)*numBots);
	complete= (bool *)malloc(sizeof(bool)*numBots);
	botLocs = (loc *)malloc(sizeof(loc) * numBots);
	int x, y; 
	for(int i=0;i<numBots;i++){
		x=rand()%width, y=rand()%height;
		botLocs[i].x=x, botLocs[i].y=y;
		for(int j=0;j<i;j++){
			if((x==botLocs[j].x&&y==botLocs[j].y)||(x==foundLoc.x&&y==foundLoc.y)){
				i--;//try again if bot was going to be placed on another bot
			}
		}
	}	
	for(int i=0;i<numBots;i++){
		printf("bot %i is at %i, %i\n", i, botLocs[i].x, botLocs[i].y);
	}
	for(int i=0;i<numBots;i++){
		tID[i] = i;
		pthread_create(&threads[i], NULL, botFunc, &(tID[i]));
	}
			pthread_barrier_wait(&barrier1);
	free(locs);
	free(botLocs);
	free(moves);
	free(complete);
	free(locChoices);
	free(dists);
}