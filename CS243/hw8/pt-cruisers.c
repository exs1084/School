#define _BSD_SOURCE
#include "racer.h"
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

int str_to_int( char str_score[] ) {//previosuly supplied method altered a bit

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

//the main program that interprets command line arguments and runs threads
//
//@param argc number of args
//@Param argv args
//@return failure or success
int main(int argc,char *argv[]){
	if(argc==1){
		fprintf(stderr, "Usage: pt-cruisers [max-speed-delay] name1 name2 [name3...]\n");
	}
	int firstN=1;//first car name
	int delay;
	if((delay=str_to_int(argv[1]))!=-1){
		initRacers((long)delay);
		firstN++;
	}else{
		initRacers((long)-1);
	}
	int numCars=0;
	if((numCars = argc-firstN)<2){
		fprintf(stderr, "Usage: pt-cruisers [max-speed-delay] name1 name2 [name3...]\n");
		return EXIT_FAILURE;
	}
	pthread_t threads[numCars];
	clear();
	srand(time(NULL));
	for(int i=0;i<numCars;i++){//make the racers here
		Racer *temp;
		if(strlen(argv[firstN+1])>6){
			printf("Error: racer names must not exceed length 6.\n");
			return EXIT_FAILURE;
		}
		temp = makeRacer(argv[firstN+i], i+1);
		assert(!(pthread_create(&threads[i], NULL, run, (void *)temp)));
	}
	void * retval;//dummy var to allow join
	for(int i=0;i<numCars;i++){
		pthread_join(threads[i], &retval);
	}
	set_cur_pos(numCars+1,0);
	return EXIT_SUCCESS;
}