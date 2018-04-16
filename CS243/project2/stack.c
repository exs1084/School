#include "stack.h"
#include "symTbl.h"
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

int emptyStack(StackNode* stack){
	if(stack==NULL){
		return 1;
	}
	return 0;
}

StackNode* makeStackNode(char* data, StackNode* next){
	//char testing[MAX_SYM_LENGTH];
	//strcpy(testing, data);
	StackNode *new;
	new = (StackNode *) malloc(sizeof(StackNode));
	if(new!=NULL){//i can read and write to both data and new->data, but not copy...
		//strcpy(new->data, data);
		new->data = data;
		new->next = next;
	}
	return new;
}

void push(StackNode** stack, char* data){
	//char temp[MAX_SYM_LENGTH];
	//strcpy(temp, data);
	if(emptyStack(*stack)){
		StackNode *new = makeStackNode(data, NULL);
		*stack = new;
	}else{
		StackNode *new = makeStackNode(data, (*stack));
		*stack = new;
	}
}

char* top(StackNode* stack){
	if(!emptyStack(stack)){
		return stack->data;
	}
	return "";
}

void pop(StackNode** stack){
	if(!emptyStack(*stack)){
		StackNode *temp;
		temp = (*stack)->next;
//printf("current: %s,", (*stack)->data);
//printf(" next: %s\n", temp->data);//segfault..
		free((*stack)->data);
		free(*stack);
		(*stack) = temp;
	}
}