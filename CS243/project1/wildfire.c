#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <time.h>
#include "display.h"
static int size=0,prob=0,dens=0,fire=0,cycles=0,cumChange=0, change=0;//prob=burn on update,dens=tree at start,fire=burn at start
static int curFire=0;
static int str_to_int( char str_score[] ) {//string to int method provided in earlier assignment
srand(time(NULL));

   int value = 0;
   for ( int j = 0; str_score[j] != '\0'; ++j ) {

      if ( isdigit( str_score[j] ) ) {
         value = value * 10 + (str_score[j] - '0');

      } else {
         // stop on finding the first non-numeric digit.
         break;   // the value is not an integer.
      }
   }
   return value;

}

static int countNeigh(int row, int col, char b[size][size]){//count how many neighbors are burning trees and return 0 for <25% and 1 for >25%
	int ret=0;//numburning
	double ret2=0.0;//numtrees total
	if(col<size-1&&row<size-1){//only checks neighbors that exist
		if(b[row+1][col+1]=='*'){
			ret++;
			ret2++;
		}
		if(b[row+1][col+1]=='Y'){
			ret2++;
		}
	}
	if(col<size-1){
		if(b[row][col+1]=='*'){
			ret++;
			ret2++;
		}
		if(b[row][col+1]=='Y'){
			ret2++;
		}
	}
	if(col<size-1&&row>0){//////
		if(b[row-1][col+1]=='*'){
			ret++;
			ret2++;
		}
		if(b[row-1][col+1]=='Y'){
			ret2++;
		}
	}
	if(row<size-1){
		if(b[row+1][col]=='*'){
			ret++;
			ret2++;
		}
		if(b[row+1][col]=='Y'){
			ret2++;
		}
	}
	if(row>0){
		if(b[row-1][col]=='*'){
			ret++;
			ret2++;
		}
		if(b[row-1][col]=='Y'){
			ret2++;
		}
	}
	if(col>0&&row<size-1){
		if(b[row+1][col-1]=='*'){
			ret++;
			ret2++;
		}
		if(b[row+1][col-1]=='Y'){
			ret2++;
		}
	}
	if(col>0){
		if(b[row][col-1]=='*'){
			ret++;
			ret2++;
		}
		if(b[row][col-1]=='Y'){
			ret2++;
		}
	}
	if(col>0&&row>0){////////
		if(b[row-1][col-1]=='*'){
			ret++;
			ret2++;
		}
		if(b[row-1][col-1]=='Y'){
			ret2++;
		}
	}
	if((ret/ret2)>.25){
		return 1;
	}
	return 0;
}

static char swap(char a,int row, int col, char b[size][size]){//decide new state
	
	if(a==' '||a=='_'){//if burned or empty, no change
		return a;
	}
	if(a=='*'){//burning already?
		if(rand()%100<prob){
			change++;
			curFire--;
			return '_';
		}
		return '*';
	}
	if(countNeigh(row, col, b)==1&&rand()%100<prob){//still alive
		change++;
		curFire++;
		return '*';
	}
	return a;

return 0;
}

static void update(char b[size][size]){//update the board
	change=0;
	char temp[size][size];
	for(int i=0;i<size;i++){
		for(int q=0;q<size;q++){
			temp[i][q]=swap(b[i][q],i,q,b);
		}
	}
	for(int i=0;i<size;i++){
		for(int q=0;q<size;q++){
			b[i][q]=temp[i][q];
		}
	}
	cumChange+=change;
}

static void fillBoard(char b[size][size]){
	int numSpace=size*size;
	int row=0, col=0;
	int numTree=numSpace*dens/100;
	int numFire=numTree*fire/100;
	curFire=numFire;
	for(int i=0;i<size;i++){
		for(int q=0;q<size;q++){//place empty space first
			b[i][q]=' ';
		}
}
while(numFire>0){
	row = rand();
	row %= size;
	col = rand();
	col %= size;//place fires next
	if(b[row][col]==' '){
		b[row][col]='*';
		numTree--;
		numFire--;
	}
}
while(numTree>0){
	row = rand();
	row %= size;
	col = rand();
	col %= size;
	if(b[row][col]==' '){//now place living trees
		b[row][col]='Y';
		numTree--;
	}
}

}

static void usage(void){//usage message

	fprintf(stderr, "usage: wildfire [-pN] size probability treeDensity proportionBurning\nThe -pN option tells the\
 simulation to print N cycles and stop.\nThe probability is the probability a tree will catch fire.\n");

}

void errorMsg(int problem){//all error messages

	switch (problem){
	case 1 :fprintf(stderr, "The -pN option was invalid.\n");
	break;
	case 2 :fprintf(stderr, "The -pN option was negative.\n");
	break;
	case 3 :fprintf(stderr, "The size (%d) must be an integer in [5...40].\n", size);
	break;
	case 4 :fprintf(stderr, "The probability (%d) must be an integer in [0...100].\n", prob);
	break;
	case 5 :fprintf(stderr, "The density (%d) must be an integer in [0...100].\n", dens);
	break;
	case 6 :fprintf(stderr, "The proportion (%d) must be an integer in [0...100].\n", fire);
	break;
	}
	usage();

}

static int checkIn(void){//verify inputs are legal and print a message if not

	if(size<5||size>40){
		errorMsg(3);
		return 0;
	}
	if(prob<0||prob>100){
		errorMsg(4);
		return 0;
	}
	if(dens<0||dens>100){
		errorMsg(5);
		return 0;
	}
	if(fire<0||fire>100){
		errorMsg(6);
		return 0;
	}
return 1;

}

static void lamePrint(char b[size][size]){//print a boring board
	for(int i=0;i<size;i++){
		for(int q=0;q<size;q++){
		printf("%c", b[i][q]);
		}
		printf("\n");
	}
	printf("cycle %d, size %d, probability %0.2f, density %0.2f, proportion %0.2f, changes %d\n", cycles, \
size, prob/100.0, dens/100.0, fire/100.0, change);
}

static void lame(int runs){//run the program in boring print mode
	char board[size][size];
	fillBoard(board);
	lamePrint(board);
	while((runs>0&&curFire>0)||cycles==0){
	update(board);
	cycles++;
	lamePrint(board);
	runs--;
	}
	printf("fires are out after %d cumulative changes.\n", cumChange);
}

static void printBoard(char board[size][size]){//print the board normally
	int row,col;
	for(row = 0; row<size; row++){
		set_cur_pos(row+1, 0);
		for(col = 0; col<size; col++){
			  put(board[row][col]);
		}
	}
	set_cur_pos(size+1,0);
	printf("cycle %d, size %d, probability %0.2f, density %0.2f, proportion %0.2f, changes %d\n", cycles, \
size, prob/100.0, dens/100.0, fire/100.0, change);
}

int main(int argc, char *argv[]){

if(argc==6){//in print mode?
	getopt(argc, argv, "p:");
	extern char *optarg;
	int runs=str_to_int(optarg);//verify print flag
	if(runs==0){
		errorMsg(1);
		return EXIT_FAILURE;
	}
	if(runs<0){
		errorMsg(2);
		return EXIT_FAILURE;
	}
	size=str_to_int(argv[2]);
	prob=str_to_int(argv[3]);
	dens=str_to_int(argv[4]);//set variables
	fire=str_to_int(argv[5]);
	if(!checkIn()){
		return EXIT_FAILURE;
	}
	lame(runs);
	return EXIT_SUCCESS;
}
if(argc!=5){//not enough inputs?
	usage();
	return EXIT_FAILURE;
}
size=str_to_int(argv[1]);
prob=str_to_int(argv[2]);//set variables for normal mode
dens=str_to_int(argv[3]);
fire=str_to_int(argv[4]);
if(!checkIn()){
	return EXIT_FAILURE;
}
char board[size][size];
fillBoard(board);
clear();
printBoard(board);
while(curFire>0){//loop for eternity(maybe)
	update(board);
	cycles++;
	printBoard(board);
	usleep(900000);
}
printf("fires are out after %d cumulative changes.\n", cumChange);
return EXIT_SUCCESS;
}
