#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>

static int str_to_int( char str_score[] ) {//string to int method provided in earlier assignment

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
//addNode
//	method to connect a new node to the tree
//args -
//	root - the root node
//	new - the new node
void addNode(TreeNode* root, TreeNode* new){
	if(root->data>new->data){//new value is smaller
		if(root->left==0){
			root->left=new;
		}else{
			addNode(root->left, new);
		}
	}else{//bigger
		if(root->right==0){
			root->right=new;
		}else{
			addNode(root->right, new);
		}
	}
}

void build_tree(TreeNode** root, const int elements[], const int count){
	
	int temp[count];
	temp[0]=(*root)->data;
	int nodes = 1;
	for(int i=1;i<count;i++){
		int repeat=0;
		for(int q=0;q<nodes;q++){//check for any repeats
			if(temp[q]==elements[i]){
				repeat=1;
			}
			
		}
		if(repeat==0){
			TreeNode *new;
			new = (TreeNode *) malloc( sizeof(TreeNode) );//allocate new node
			new->data=elements[i];
			new->left=0;
			new->right=0;
			addNode(*root, new);//connect nodes
			temp[nodes]=elements[i];//add new data to repeat check array
			nodes++;
			
		}
	}
}
//traverse the tree
void traverse(const TreeNode* root, const TraversalType type){
	if(type==PREORDER){//print, left, right
		printf("%d\n", root->data);
		if((root->left)!=0){
			traverse(root->left, type);
		}
		if((root->right)!=0){
			traverse(root->right, type);
		}
	}//left, print, right
	if(type==INORDER){
		if((root->left)!=0){
			traverse(root->left, type);
		}
		printf("%d\n", root->data);
		if((root->right)!=0){
			traverse(root->right, type);
		}
	}
	if(type==POSTORDER){//left, right, print
		if((root->left)!=0){
			traverse(root->left, type);
		}
		if((root->right)!=0){
			traverse(root->right, type);
		}
		printf("%d\n", root->data);
	}
}

void cleanup_tree(TreeNode* root){
	if((root->left)!=0){
		cleanup_tree(root->left);
	}
	if((root->right)!=0){
		cleanup_tree(root->right);
	}
	free(root);
}
//main
int main(int argc, char *argv[]){
	if(argc!=2){
		printf("Usage: bst #\n");//wrong number of arguments
		return EXIT_FAILURE;
	}
	int nums = str_to_int(argv[1]);
	if(!nums>0){//check argument validity
		printf("# must be greater than 0\n");
		return EXIT_FAILURE;//bad argument
	}
	int elements[nums];
	printf("Enter %d integer values to place in tree:\n", nums);
	for(int i=0;i<nums;i++){
		char buffer[10];//read in numbers, up to 10 digit numbers
		fgets(buffer, 10, stdin);
		elements[i]=str_to_int(buffer);
	}
	printf("Input values:\n");//reprint inputs
	for(int i=0;i<nums;i++){
		printf("%d\n", elements[i]);
	}
	TreeNode* root;
	(root) = (TreeNode *) malloc( sizeof(TreeNode) );//allocate for root
	(root)->data=elements[0];
	(root)->left=0;//initializing root
	(root)->right=0;
	build_tree(&root, elements, nums);//build the tree
	printf("Preorder:\n");
	traverse(root, PREORDER);//print the three traversals
	printf("Inorder:\n");
	traverse(root, INORDER);
	printf("Postorder:\n");
	traverse(root, POSTORDER);
	cleanup_tree(root);//free the memory
	return EXIT_SUCCESS;
}
