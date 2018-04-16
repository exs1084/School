//
//  Rasterizer.cpp
//
//  Created by Joe Geigel on 11/30/11.
//  Modifications by Warren R. Carithers.
//  Copyright 2011 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//

#include <list>
#include <algorithm>
#include <iostream>

#include "Rasterizer.h"
#include "Canvas.h"

using namespace std;

struct bucket{
	int ymax;
	float x;
	float invM;
	bucket *link = NULL;
};
///
// Simple class that performs rasterization algorithms
///

///
// Constructor
//
// @param n number of scanlines
// @param C The Canvas to use
///
Rasterizer::Rasterizer( int n, Canvas &canvas ) : n_scanlines(n), C(canvas)
{
}


void sortedInsert(bucket** head_ref, bucket* new_node)
{
    bucket* current;
    /* Special case for the head end */
    if (*head_ref == NULL)
    {
        new_node->link = *head_ref;
        *head_ref = new_node;
        return;
    }
    if((*head_ref)->x >= new_node->x){
        new_node->link = *head_ref;
        *head_ref = new_node;
        return;
	}
    if((*head_ref)->x < new_node->x){
        /* Locate the node before the point of insertion */
        current = *head_ref;
        while (current->link!=NULL &&
               current->link->x < new_node->x)
        {
            current = current->link;
        }
        new_node->link = current->link;
        current->link = new_node;
        return;
    }
}

void insertionSort(bucket **head_ref, int y)
{
    // Initialize sorted linked list
    bucket *sorted = NULL;
 
    // Traverse the given linked list and insert every
    // node to sorted
    bucket *current = *head_ref;
    while (current != NULL)
    {
        // Store next for next iteration
        bucket *next = current->link;
 
        // insert current in sorted linked list
        if(y != current->ymax){
			sortedInsert(&sorted, current);
		}else{
			delete current;
		}
        // Update current
        current = next;
    }
 
    // Update head_ref to point to sorted linked list
    *head_ref = sorted;
    bucket *loop = sorted;
    int o = 0;
    cout<<"sorted list pointers are:   "<<loop<<"\n";
    while(loop!=NULL){
		cout<<loop->link<<"\n";
		loop = loop->link;
	}
	cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}



///
// Draw a filled polygon.
//
// Implementation should use the scan-line polygon fill algorithm
// discussed in class.
//
// The polygon has n distinct vertices.  The coordinates of the vertices
// making up the polygon are stored in the x and y arrays.  The ith
// vertex will have coordinate (x[i],y[i]).
//
// You are to add the implementation here using only calls to the
// setPixel() function.
//
// @param n - number of vertices
// @param x - array of x coordinates
// @param y - array of y coordinates
///
void Rasterizer::drawPolygon(int n, const int x[], const int y[] )
{
    // YOUR IMPLEMENTATION HERE
    //bucket ET[n_scanlines];//tempory holding list
    bucket *ET [600] = {NULL};
    bucket temp [n];
    int ymin=0;
    int xMax = 0;
    int maxY = 0;
    for(int i=0;i<n;i++){//build edge table
																					C.setPixel(x[i],y[i]);
		if(y[i]>maxY){
			maxY = y[i];
		}
		if(i==(n-1)){//last edge
			if(y[i]==y[0]){//dont make bucket, horizontal
				
			}else{
				if(y[i]>y[0]){//determine ymax
					temp[i].ymax=y[i];
					temp[i].x=x[0];
					ymin=y[0];
				}else{
					temp[i].ymax=y[0];
					temp[i].x=x[i];
					ymin = y[i];
				}	
				temp[i].invM = (float)(x[0]-x[i])/(float)(y[0]-y[i]);
				//cout << "insert";
				//cout << "temp address  " << test << "\n";
				//ET[ymin] = appendBuck(ET[ymin], &(temp[i]));
				sortedInsert(&(ET[ymin]), &(temp[i]));
			}
		}
		if(y[i]==y[i+1]){
			
		}else{
			if(y[i]>y[i+1]){//determine ymax
				temp[i].ymax=y[i];
				temp[i].x=x[i+1];
				ymin=y[i+1];
			}else{
				temp[i].ymax=y[i+1];
				temp[i].x=x[i];
				ymin=y[i];
			}
			temp[i].invM = (float)(x[i]-x[i+1])/(float)(y[i]-y[i+1]);
			//ET[ymin] = appendBuck(ET[ymin], &(temp[i]));//insert edge
			sortedInsert(&(ET[ymin]), &(temp[i]));
		
		}
	}//ET built
	
	cout<<"temp bucket array contents \n";
	for(int i=0;i<n;i++){
		cout<<&(temp[i])<<"\n";
		cout<<(temp[i]).link<<"\n";
	}
	cout<<"ET220      "<<ET[220]<<"\n";
	cout<<"ET220 link "<<(ET[220])->link<<"\n";
	bucket *AL = NULL;
	bool draw = false;
	int yCurr = 0;
	int done = 0;
	for(int i=0;i<600;i++){//900X600 window---find ymin
		if(ET[i]!=NULL){
																												cout << "start drawing \n";
																												cout <<ET[i]<<"  etI : link   "<<(ET[i])->link<<"\n";
			yCurr=i;
			//cout<<ET[i]<<"SET \n";
			break;
		}
	}
	//cout<<"value to input "<<ET[yCurr]<<" \n";
	maxY = yCurr +4;
	while(yCurr<maxY){//loop drawing 
		if(ET[yCurr]!=NULL){
			sortedInsert(&AL, ET[yCurr]);
			if(AL == AL->link){
																								cout<<"					self reference from start \n";//is not always from the start
			}
																						//cout << AL << "  " << ET[yCurr] << "\n";
		}
		insertionSort(&AL, yCurr);
		//cout<<AL<<"  "<<"\n";
		bucket *left = AL;
		bucket *right = AL->link;
																						//cout<<left<<"   "<<right<<"\n";
		while(right!=NULL){//draw line------not exiting when right is null...
			for(int i=left->x;i<right->x;i++){
				C.setPixel(i,yCurr);
			}
			//cout<<left->invM<<"    "<<right->invM<<"\n";
			left->x += (left->invM);
			right->x += (right->invM);
			if(right->link==NULL||left->link==NULL){
				break;
			}
			if(right->link==left->link){
																										//cout<<"self reference \n";//not having this breaks while loop on second shape
				break;
			}
																									//cout<<"valid ref, next part \n";
			left = right->link;
			right = left->link;
		}
		yCurr++;
																								cout<<yCurr<<"   curr : max   "<<maxY<<"\n";
	}
	for(int i=0;i<n;i++){
		//delete &(temp[i]);//sometimes causes core dump?!?!
	}
}

