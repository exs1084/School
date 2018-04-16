#include "stack.h"
#include "stackNode.h"
#include <stdio.h>


int main(void){
	StackNode* stack;
	push(&stack, "tree");
	printf("%s \n", top(stack));
	push(&stack, "fire");
	printf("%s \n", top(stack));
	pop(&stack);
	printf("%s \n", top(stack));
	pop(&stack);
}