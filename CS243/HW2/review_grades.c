#include <ctype.h>
#include <stdio.h>
#include "stats.h"
#include <stdlib.h>

/// str_to_int converts the input C string to an integer value.
/// @param str_score the NUL-terminated C String to convert
/// @return integer value of str_score
///
int str_to_int( char str_score[] ) {

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

int main(int argc, char *argv[]){
argc--;
if(argc==0){
fprintf(stderr, "usage: review_grades score1 [score2 ...]\n");
fprintf(stderr, "note: score values assumed to be non-negative.\n");
return EXIT_FAILURE;
}
printf("population: %d\n", argc);
int argvv[argc];
for(int i=0;i<argc;i++){
argvv[i]=str_to_int(argv[i+1]);
}
for(int i=0;i<argc;i++){
printf("%d ", argvv[i]);
}
printf("\n");
double mean = average(argc, argvv);
printf("mean: %f\n", mean);
printf("std deviation: %100.5f\n", std_dev(argc, argvv, mean));
histogram(argc, argvv);
return EXIT_SUCCESS;
}
