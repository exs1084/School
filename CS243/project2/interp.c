#include "interp.h"
#include "symTbl.h"
#include "stack.h"
#include "stackNode.h"
#include "parser.h"
#include "expNode.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>

static char input[MAX_LINE];
static int quit=0;

int main( int argc, const char* argv[] ){
	if(argc>2){
		printf("Usage: interp [sym-table]\n");
		return EXIT_FAILURE;
	}
	if(argc==2){
		char tab[50];
		strcpy(tab, argv[1]);
		buildTable(tab);
	}
	printf("SYMBOL TABLE:\n");
	dumpTable();
	printf("Enter postfix expressions (CTRL-D to exit):\n");
	while(!quit){//press ctrl d?
		printf("> ");
		quit=(fgets(input, MAX_LINE, stdin) == NULL);
		if(!quit){
			parse(input);
		}
		//parser stuff goes here
	}
	printf("\n");
	printf("SYMBOL TABLE:\n");
	dumpTable();
	return EXIT_SUCCESS;
}