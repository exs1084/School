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

static ParserError error = NONE;

ParserError getParserError(void){
	return error;
}

void cleanupTree(ExpNode* node){
	if((node->left)!=0){
		cleanupTree(node->left);
	}
	if((node->right)!=0){
		cleanupTree(node->right);
	}
	free(node);
}

void printOp(ExpType op){//print the string form for operator nodes
	if(op==ADD_OP){
		printf(" %s ", ADD_OP_STR);
	}else if(op==SUB_OP){
		printf(" %s ", SUB_OP_STR);
	}else if(op==MUL_OP){
		printf(" %s ", MUL_OP_STR);
	}else if(op==DIV_OP){
		printf(" %s ", DIV_OP_STR);
	}else if(op==MOD_OP){
		printf(" %s ", MOD_OP_STR);
	}else{
		printf(" %s ", ASSIGN_OP_STR);
	}
}

void infixTree(ExpNode* node){
	ExpType temp = node->type;
	if(temp!=DOUBLE&&temp!=INTEGER&&temp!=SYMBOL){
		printf("(");
		infixTree(node->left);
		printOp(temp);
		infixTree(node->right);
		printf(")");
	}else if(temp==SYMBOL){
		printf("%s", node->symbol);
	}else if(temp==INTEGER){
		printf("%d", node->value.value.iVal);
	}else{
		printf("%f", node->value.value.dVal);
	}
}

Value evalTree(ExpNode* node){
	ExpType temp = node->type;
	Value ret = {TYPE_UNKNOWN, {0}};
	if(temp==INTEGER||temp==DOUBLE||temp==SYMBOL){
		ret = node->value;
		return ret;
	}
	if(node->right==NULL||node->left==NULL){
		error = TOO_FEW_TOKENS;
		printf("Invalid expression, not enough tokens\n");
		return ret;
	}
	Value right = evalTree(node->right);


	if(temp==ASSIGN_OP){
		if(node->left->type!=SYMBOL&&node->left->type!=UNKNOWN){
			printf("Invalid assignment\n");
			error = INVALID_ASSIGNMENT;
			return ret;
		}
		Symbol *check;
		if((check=lookupTable(node->left->symbol))==NULL){//symbol not in table
			Symbol new = {"temp00000000000", right};//create symbol to add
			strcpy(new.varName, node->left->symbol);
			if((addTable (new))==0){//cannot be added
				error=SYMBOL_TABLE_FULL;
				return ret;
			}
			ret = new.value;
			node->left->type=SYMBOL;
			//is added
		}else{//is in table
			if(check->value.type!=right.type){//assignment mismatch?
				error=INVALID_ASSIGNMENT;
				return ret;
			}else{//change the value
				if(right.type==TYPE_INT){
					check->value.value.iVal = right.value.iVal;
				}else{
					check->value.value.dVal = right.value.dVal;
				}
			}
			ret = check->value;
		}
	}


	Value left = evalTree(node->left);
	if(left.type==TYPE_UNKNOWN||right.type==TYPE_UNKNOWN){
		return ret;
	}
	if(temp==ADD_OP){
		if(left.type==TYPE_INT&&right.type==TYPE_INT){//both int
			ret.type=TYPE_INT;
			ret.value.iVal = left.value.iVal + right.value.iVal;
		}else{//not both int
			ret.type=TYPE_DOUBLE;
			if(left.type==TYPE_INT&&right.type==TYPE_DOUBLE){
				ret.value.dVal = left.value.iVal + right.value.dVal;
			}else if(left.type==TYPE_DOUBLE&&right.type==TYPE_INT){
				ret.value.dVal = left.value.dVal + right.value.iVal;
			}else{
				ret.value.dVal = left.value.dVal + right.value.dVal;
			}
		}
	}
	if(temp==SUB_OP){
		if(left.type==TYPE_INT&&right.type==TYPE_INT){//both int
			ret.type=TYPE_INT;
			ret.value.iVal = left.value.iVal - right.value.iVal;
		}else{//not both int
			ret.type=TYPE_DOUBLE;
			if(left.type==TYPE_INT&&right.type==TYPE_DOUBLE){
				ret.value.dVal = left.value.iVal - right.value.dVal;
			}else if(left.type==TYPE_DOUBLE&&right.type==TYPE_INT){
				ret.value.dVal = left.value.dVal - right.value.iVal;
			}else{
				ret.value.dVal = left.value.dVal - right.value.dVal;
			}
		}
	}
	if(temp==MUL_OP){
		if(left.type==TYPE_INT&&right.type==TYPE_INT){//both int
			ret.type=TYPE_INT;
			ret.value.iVal = left.value.iVal * right.value.iVal;
		}else{//not both int
			ret.type=TYPE_DOUBLE;
			if(left.type==TYPE_INT&&right.type==TYPE_DOUBLE){
				ret.value.dVal = left.value.iVal * right.value.dVal;
			}else if(left.type==TYPE_DOUBLE&&right.type==TYPE_INT){
				ret.value.dVal = left.value.dVal * right.value.iVal;
			}else{
				ret.value.dVal = left.value.dVal * right.value.dVal;
			}
		}
	}
	if(temp==DIV_OP){
		if((right.type==TYPE_INT&&right.value.iVal==0)\
		||(right.type==TYPE_DOUBLE&&right.value.dVal==0)){
			error = DIVISION_BY_ZERO;
			printf("Division by zero\n");
			return ret;
		}
		if(left.type==TYPE_INT&&right.type==TYPE_INT){//both int

			ret.type=TYPE_INT;
			ret.value.iVal = left.value.iVal / right.value.iVal;
		}else{//not both int
			ret.type=TYPE_DOUBLE;
			if(left.type==TYPE_INT&&right.type==TYPE_DOUBLE){
				ret.value.dVal = left.value.iVal / right.value.dVal;
			}else if(left.type==TYPE_DOUBLE&&right.type==TYPE_INT){
				ret.value.dVal = left.value.dVal / right.value.iVal;
			}else{
				ret.value.dVal = left.value.dVal / right.value.dVal;
			}
		}
	}
	if(temp==MOD_OP){
		if(!(left.type==TYPE_INT&&right.type==TYPE_INT)){
			printf("Modulus requires both types to be int\n");
			error = INVALID_MODULUS;
			return ret;
		}
		if(right.value.iVal==0){
			error = DIVISION_BY_ZERO;
			printf("Division by zero\n");
			return ret;
		}
		ret.type=TYPE_INT;
		ret.value.iVal = left.value.iVal % right.value.iVal;
	}

	return ret;
}
//add a node to the tree based on presence of operator nodes in the tree
//uses recursion to place in correct location.
//
//@param root the root of the tree
//@param new the new node
//@return 0 if not placed in tree, 1 if placed
//
int addNode(ExpNode** root, ExpNode **new){
	if((*root)==NULL){
		(*root) = (*new);
		return 1;
	}
	if((*root)->type==INTEGER||(*root)->type==DOUBLE||(*root)->type==SYMBOL){//nonop
		return 0;//node not placed
	}
	if(addNode(&((*root)->right), new)==1){//place from right?
		return 1;
	}
	if(addNode(&((*root)->left), new)==1){
		return 1;
	}
	error=TOO_MANY_TOKENS;
	return 0;
}

ExpNode* parseTree(char expr[]){
	char *toScan = expr;
	int offset;
	StackNode *stack = NULL;
	char tempC[MAX_SYM_LENGTH];
	int read=2;
	while((read=sscanf(toScan,"%s%n", tempC, &offset))>0){//read tokens and make nodes
	//char pushStr[MAX_SYM_LENGTH];
	//strcpy(pushStr, tempC);
//shorten temp each loop
		char *pushC = (char*)malloc(MAX_SYM_LENGTH * sizeof(char));
		strcpy(pushC, tempC);
		push((&stack), pushC);
		toScan += offset;
	}//now make a tree
	ExpNode *root = NULL;
	while(!emptyStack(stack)){
		ExpNode *tempNode = NULL;
		//char* testing =top(stack);
		char testing[MAX_SYM_LENGTH];
		strcpy(testing, top(stack));
		tempNode = makeExpNode(testing ,NULL, NULL);
		if(addNode((&root), (&tempNode))==0){
			return NULL;
		}
		pop(&stack);
	}
	return root;
}

void parse(char exp[]){
	error = NONE;
	ExpNode* tree = NULL;
	if((tree = parseTree(exp))==NULL){
		printf("Invalid expression, too many tokens\n");
		return;
	}
	Value result = evalTree(tree);
	if(getParserError()!=NONE){
		return;
	}
	infixTree(tree);//print result
	if(result.type==TYPE_INT){
		printf(" = %d\n", result.value.iVal);
	}else{
		printf(" = %f\n", result.value.dVal);
	}
	cleanupTree(tree);
}