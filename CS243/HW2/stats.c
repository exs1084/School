

#include <math.h>
#include <stdio.h>
#include "stats.h"

double average( int count, const int scores[]){
double sum = 0;
for( int i=0;i<count;i++){
sum = sum+scores[i];
}
return sum/count;
}

double std_dev( int count, const int scores[], double mean ){
int N = count-1;
double summation = 0;
for(int i=0;i<count;i++){
double temp = scores[i]-mean;
summation+=temp*temp;
}
double frac = summation/N;
double ret = sqrt(frac);
return ret;
}

void histogram( int count, const int scores[] ){
int grades[10];
int check;
for(int i=0;i<count;i++){
int temp = scores[i];
check=0;
if(temp<60&&check==0){//F
grades[9]++;
check=1;
}

if(temp<69&&check==0){//D
grades[8]++;
check=1;
}

if(temp<72&&check==0){//C-
grades[7]++;
check=1;
}

if(temp<75&&check==0){//C
grades[6]++;
check=1;
}

if(temp<79&&check==0){//C+
grades[5]++;
check=1;
}

if(temp<82&&check==0){//B-
grades[4]++;
check=1;
}

if(temp<85&&check==0){//B
grades[3]++;
check=1;
}

if(temp<89&&check==0){//B+
grades[2]++;
check=1;
}

if(temp<92&&check==0){//A-
grades[1]++;
check=1;
}

//A
if(check==0){
grades[0]++;
}
}
printf("A:\t%d\n", grades[0]);
printf("A-:\t%d\n", grades[1]);
printf("B+:\t%d\n", grades[2]);
printf("B:\t%d\n", grades[3]);
printf("B-:\t%d\n", grades[4]);
printf("C+:\t%d\n", grades[5]);
printf("C:\t%d\n", grades[6]);
printf("C-:\t%d\n", grades[7]);
printf("D:\t%d\n", grades[8]);
printf("F:\t%d\n", grades[9]);

}
