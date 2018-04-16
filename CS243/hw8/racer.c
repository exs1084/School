#define _BSD_SOURCE
#include "racer.h"
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

long maxDelay =DEFAULT_WAIT;
pthread_mutex_t lock;

void initRacers( long milliseconds ){
	assert(!(pthread_mutex_init(&lock, NULL)));
	if(milliseconds==-1){
		return;
	}
	maxDelay = milliseconds;
}

Racer *makeRacer( char *name, int position ){
	Racer *ret;
	ret = (Racer *)malloc(sizeof(Racer));
	ret->row=position;
	ret->dist=0;
	char *car = (char *)malloc(MAX_CAR_LEN+1);//might have to do char *
	strcpy(car, "~O=-------o>");
	for(unsigned int i=0;i<strlen(name);i++){
		car[3+i] = name[i];
	}
	ret->graphic = car;
	return ret;
}

void destroyRacer( Racer *racer ){
	free(racer->graphic);
	free(racer);
}

void *run( void *racer ){
	assert(racer!=NULL);
	Racer *this = (Racer *)racer;
	int flat=0;
	pthread_mutex_lock(&lock);//print at start
	set_cur_pos(this->row, this->dist);
	for(int i=0;i<MAX_CAR_LEN;i++){
		put(this->graphic[i]);
	}
		pthread_mutex_unlock(&lock);
	while(!flat){
		int speed = rand() % maxDelay;
		if(speed<4){
			this->graphic[1]='X';
			flat=1;
			this->dist--;//so that it stays put this time
		}
		this->dist++;
		pthread_mutex_lock(&lock);
		set_cur_pos(this->row, 0);//clear line
		for(int i=0;i<FINISH_LINE;i++){
				put(' ');
		}
		set_cur_pos(this->row, this->dist);//draw car
		for(int i=0;i<MAX_CAR_LEN;i++){
			put(this->graphic[i]);
		}
		pthread_mutex_unlock(&lock);
		if(FINISH_LINE==this->dist){
			break;
		}
		usleep(speed);
	}
	destroyRacer(this);
	return NULL;
}