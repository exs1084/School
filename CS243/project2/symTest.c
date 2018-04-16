#include "symTbl.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>




int main(void){
	char txt[] = {"symIn.txt"};
printf("build\n");
	buildTable(txt);
printf("dump\n");
	dumpTable();
printf("lookup\n");
	Symbol *test = lookupTable("a");
	printf("lookup name: %s value: %d\n", (*test).varName, (*test).value.value.iVal);
}