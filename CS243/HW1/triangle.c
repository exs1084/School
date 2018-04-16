/// 
/// File: triangle.c 
/// 
/// A program to print out multiple triangles "right-up" triangles.
/// of the form:
///     *
///   ***
/// *****
/// 
/// @author TODO_AUTHOR_ID_ : Erikson Sodergren
/// 
// // // // // // // // // // // // // // // // // // // // // // // // 

// TODO_ADD_#INCLUDES_ HERE
#include <stdio.h>
/**
 *  Function: drawTriangle
 *
 *  Description: function to draw a triangle whose height/width is "size"
 * 
 *  @param size  the size of the triangle to draw
 */
// TODO_WRITE_DRAWTRIANGLE_FUNCTION HERE
void drawTriangle(int size){
if(size%2==0)
{
int temp = size+2;
size=temp/2;
}else
{
int temp = size+1;
size=temp/2;
}
int total=(2*size)-1;
for(int i=1;i<=size;i++){
int ast=(2*i)-1;
for(int w=0;w<total-ast;w++)
{
printf(" ");
}
for(int b=1;b<=ast;b++){
printf("*");
}
printf("\n");
}
printf("\n");
}	
/**
 *  Function: main 
 *
 *  Description: program to draw 3 triangles of size 1, 4, and 6.
 *
 *  @returns errorCode  error Code; EXIT_SUCCESS if no error
 */
// TODO_WRITE_MAIN_FUNCTION HERE
int main(void)
{
drawTriangle(1);
drawTriangle(4);
drawTriangle(6);
return 0;
}
// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
