#include "expNode.h"
#include "symTbl.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>

ExpNode* makeExpNode(char token[], ExpNode* left, ExpNode* right){
	char test[MAX_SYM_LENGTH];
	strcpy(test, token);
	ExpNode *new;
	new = (ExpNode *)malloc(sizeof(ExpNode));
	new->right=right;
	new->left=left;
	
	if(!strcmp(test, ADD_OP_STR)){//operators check
		new->type = ADD_OP;
		return new;
	}else if(!strcmp(test, SUB_OP_STR)){
		new->type = SUB_OP;
		return new;
	}else if(!strcmp(test, MUL_OP_STR)){
		new->type = MUL_OP;
		return new;
	}else if(!strcmp(test, DIV_OP_STR)){
		new->type = DIV_OP;
		return new;
	}else if(!strcmp(test, MOD_OP_STR)){
		new->type = MOD_OP;
		return new;
	}else if(!strcmp(test, ASSIGN_OP_STR)){
		new->type = ASSIGN_OP;
		return new;
	}else if(isalpha(test[0])){//now check for symbols
		new->type = SYMBOL;
		strcpy(new->symbol, test);
		Symbol* temp = lookupTable(test);
		if(temp==NULL){
			new->type = UNKNOWN;
		}else{
			new->value = temp->value;
		}
		return new;
	}else{//now check for doubles
		for(int i=0;i<MAX_SYM_LENGTH;i++){
			if('.'==token[i]){
				new->type = DOUBLE;
				new->value.type=TYPE_DOUBLE;
				sscanf(test, "%lf", &(new->value.value.dVal));
				return new;
			}
		}
	}
	new->type = INTEGER;
	new->value.type=TYPE_INT;
	sscanf(test, "%d", &(new->value.value.iVal));
	return new;
}