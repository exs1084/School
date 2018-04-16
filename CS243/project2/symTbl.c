#include "symTbl.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>

static Symbol table[MAX_SYMBOLS];
static int currSyms=0;

void buildTable(char filename[]){
	FILE*in;
	if((in=fopen(filename, "r"))==NULL){
	fputs("Error loading symbol table\n", stderr);
	exit( EXIT_FAILURE );
	}
	int read=1;
	char tempName[MAX_SYM_LENGTH];
	char sType[MAX_SYM_LENGTH];
	while((read = fscanf(in, "%s %s", sType, tempName))>0){
		if(strcmp("int", sType)==0){
			int val;
			fscanf(in, "%d", &val);
			Value tempVal = {TYPE_INT, {val}};
			Symbol tempSym = {"temp00000000000", tempVal};
			strcpy(tempSym.varName, tempName);
			addTable(tempSym);
		}
		if(strcmp("double", sType)==0){
			double val;
			fscanf(in, "%lf", &val);
			Value tempVal = {TYPE_DOUBLE, {val}};
			tempVal.value.dVal=val;//had issues with dVal being 0 without this line
			Symbol tempSym = {"temp00000000000", tempVal};
			strcpy(tempSym.varName, tempName);
			addTable(tempSym);
		}
	}
}

void dumpTable(void){
	for(int i=0;i<currSyms;i++){
		char tempName[MAX_SYM_LENGTH];
		strncpy(tempName, table[i].varName, sizeof(tempName));
		Type tempType = table[i].value.type;
		if(tempType==TYPE_INT){
			int tempVal = table[i].value.value.iVal;
			printf("\tType: int Name: %s Value: %d\n", tempName, tempVal);
		}else{
			double tempVal = table[i].value.value.dVal;
			printf("\tType: double Name: %s Value: %lf\n", tempName, tempVal);
		}
	}
}

Symbol* lookupTable(char variable[]){
	for(int i=0;i<currSyms;i++){
		if(strcmp(table[i].varName, variable)==0){
			return &(table[i]);
		}
	}
	return NULL;
}

int addTable(Symbol symbol){
	if(currSyms>=MAX_SYMBOLS){
		return 0;
	}
	table[currSyms]=symbol;
	currSyms++;
	return 1;
}