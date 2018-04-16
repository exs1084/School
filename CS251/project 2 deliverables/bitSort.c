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

#define maxN 16384

typedef struct{
	int tID;
	int start;
	int len;
	bool ascend;
	int tCount;
}parg;

int numThreads = 1;//1, 2, 4, 8
//pthread_t threads[8];
//parg dat[8];
int N=1;
int data[maxN];

pthread_mutex_t mutex1;
pthread_barrier_t barrier1;

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

void printDat(){
	for(int i=0;i<N;i++){
		printf("%i ", data[i]);
	}
	printf("\n");
}

void swap(int i, int j){
	//printf("swapping %i and %i \n", i, j);
	int temp = data[i];
	data[i] = data[j];
	data[j] = temp;
}

void sortSer(int start, int ascend, int len){
	for(int z=2;z<=len;z*=2){//phases
		for(int j=start;j<start+len;j+=(2*len)/z){//blocks
			for(int i=j; i<j+len/z;i++){//comparisons
				if((data[i] > data[i+len/z]) && ascend){//ascending
					swap(i, i+len/z);
				}
				if((data[i] < data[i+len/z]) && !ascend){//descending
					swap(i, i+len/z);
				}
			}
		}
	}
}

void *tFunc1(void *input){
	parg *args = (parg *)input;
	int start = args->start;
	int len = args->len;
	bool ascend = args->ascend;
	int tCount = args->tCount;
//	printf("func1, start=%i, length=%i\n", start, len);
	for(int i=start; i<start+len;i+= numThreads/tCount){//comparisons
		if((data[i] > data[i+len]) && ascend){//ascending
			swap(i, i+len);
		}
		if((data[i] < data[i+len]) && !ascend){//descending
			swap(i, i+len);
		}
	}
}

void *tFunc2(void *input){
	parg *args = (parg *)input;
	int start = args->start;
	int len = args->len;
	bool ascend = args->ascend;
	int tID = args->tID;
	int tCount = args->tCount;
	pthread_t threads[8];
	parg dat[8];
	
//printf("func2 tID=%i, start = %i, length = %i, numThreads=%i, tCount=%i\n",tID, start, len, numThreads, tCount);
	for(int z=2;z<=len;z*=2){
		if(numThreads==tCount){//just keep going
			for(int j=start;j<start+len;j+=(2*len)/z){//blocks
				for(int i=j; i<j+len/z;i++){//comparisons
					if((data[i] > data[i+len/z]) && ascend){//ascending
						swap(i, i+len/z);
					}
					if((data[i] < data[i+len/z]) && !ascend){//descending
						swap(i, i+len/z);
					}
				}
			}
		}else{//double threads and tCount if not using all
			if(z==2){//use multiple threads in this block


//maybe needs to be checked over and compared to pass 1 calls to func1...other parts of func2 are wrong as well though


				for(int i=0;i<numThreads/tCount;i++){
					dat[i].start = start + i;
					dat[i].len = len/2;
					dat[i].ascend = ascend;
					dat[i].tCount = tCount;
					pthread_create(&threads[i], NULL, tFunc1, &(dat[i]));
				}
				for(int t=0;t<numThreads/tCount;t++){
					pthread_join(threads[t], NULL);
				}
			}else{//split into 2 threads
				dat[tID].tID=tID;
				dat[tID].start = start;
				dat[tID].len = len/2;
				dat[tID].ascend = ascend;
				dat[tID].tCount = 2*tCount;
				dat[tID+tCount].tID=tID+tCount;
				dat[tID+tCount].start = start+len/2;
				dat[tID+tCount].len = len/2;
				dat[tID+tCount].ascend = ascend;
				dat[tID+tCount].tCount = 2*tCount;

				pthread_create(&threads[tID], NULL, tFunc2, &(dat[tID]));
				pthread_create(&threads[tID+tCount], NULL, tFunc2, &(dat[tID+tCount]));

				pthread_join(threads[tID], NULL);
				pthread_join(threads[tID+tCount], NULL);
			}
		}
	}
}

void sortPar(int start, int ascend, int len){
	pthread_t threads[8];
	parg dat[8];
	for(int z=2;z<=4;z*=2){//phases
		if(z==2){//first pass---working correctly
//printf("first pass\n");
			for(int i=0;i<numThreads;i++){
				dat[i].start = start + i;
				dat[i].len = len/2;
				dat[i].ascend = ascend;
				dat[i].tCount = 1;
				pthread_create(&threads[i], NULL, tFunc1, &(dat[i]));
			}
			for(int t=0;t<numThreads;t++){
				pthread_join(threads[t], NULL);
			}//printDat();
		}else{//second pass--not working
//printf("second pass\n");
			dat[0].tID=0;
			dat[0].start = start;
			dat[0].len = len/2;
			dat[0].ascend = ascend;
			dat[0].tCount = 2;

			dat[1].tID=1;
			dat[1].start = start+len/2;
			dat[1].len = len/2;
			dat[1].ascend = ascend;
			dat[1].tCount = 2;

			pthread_create(&threads[0], NULL, tFunc2, &(dat[0]));
			pthread_create(&threads[1], NULL, tFunc2, &(dat[1]));

			pthread_join(threads[0], NULL);
			pthread_join(threads[1], NULL);
		}
	}
}

void makeBitSer(int len){
	bool ascend;
	for(int size = 2; size<len; size*=2){//loops to create bitonic list
		ascend = true;
		for(int i=0;i<len;i+=size){
			//printf("sort from %i with size %i \n", i, size);
			sortSer(i, ascend, size);
			if(ascend==true){
				ascend=false;}else{
				ascend=true;
			}
		}
		//printDat();
	}
}

//the main program that interprets command line arguments and runs threads
//
//@param argc number of args
//@Param argv args
//@return failure or success
int main(int argc,char *argv[]){
	clock_t start, end;
	char input[10];
	scanf("%s", input);
	numThreads = str_to_int(input);
//printf("numthreads = %i\n", numThreads);
	scanf("%s", input);
	N = str_to_int(input);
//printf("N = %i\n", N);
	for(int i=0;i<N;i++){
		scanf("%s", input);
		data[i] = str_to_int(input);
	}
//printf("data in array\n");
	start = clock();
	makeBitSer(N);//turn into bitonic
//printf("bitonic\n");
	//printf("~~~bitonic sequence~~~\n");
	//printDat();
	if(numThreads==1){
		sortSer(0, true, N);//call bitonic sort
	}else{
		sortPar(0, true, N);
	}
	end = clock();
	printf(" time elapsed: %f \n", (double)(end - start)/CLOCKS_PER_SEC);
	for(int i=0;i<N-1;i++){
		if(data[i]>data[i+1]){
			printf("incorrect sort\n");
		}
	}
	printf("###finished sorting###\n");
	printDat();
}