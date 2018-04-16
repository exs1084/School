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
	float ymax;
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

void addBuck (bucket** head_ref, bucket* new_node){
	bucket *temp = *head_ref;
	if(temp==NULL){
		*head_ref = new_node;
		return;
	}
	while(temp->link!=NULL){
		temp=temp->link;
	}
	temp->link = new_node;
	
}

void sortedInsert(bucket** head_ref, bucket* new_node)
{
    bucket* current;
    /* Special case for the head end */
    if (*head_ref == NULL)
    {
		new_node->link = NULL;
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

void insertionSort(bucket **head_ref, float y)
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
			//delete current;
		}
        // Update current
        current = next;
    }
 
    // Update head_ref to point to sorted linked list
    *head_ref = sorted;
    bucket *loop = sorted;
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
void Rasterizer::drawPolygon(int n, Vertex in[] )
{
	int x[n];
	int y[n];
	for(int i=0;i<n;i++){
		x[i] = (int)in[i].x;
		y[i] = (int)in[i].y;
	}
	
	
	
	
    // YOUR IMPLEMENTATION HERE
    bucket *ET [600] = {NULL};
    bucket temp [n];
    int ymin=0;
    int xMax = 0;
    int maxY = 0;
    for(int i=0;i<n;i++){//build edge table
		if(y[i]>maxY){
			maxY = y[i];
		}
		if(i==(n-1)){//last edge
			if(y[i]==y[0]){//dont make bucket, horizontal
				
			}else{
				if(y[i]>y[0]){//determine ymax
					temp[i].ymax=y[i];
					temp[i].x=x[0];
					ymin = y[0];
				}else{
					temp[i].ymax=y[0];
					temp[i].x=x[i];
					ymin = y[i];
				}	
				temp[i].invM = (float)(x[0]-x[i])/(float)(y[0]-y[i]);
				addBuck(&(ET[ymin]), &(temp[i]));//insert into ET
			}
		}else{
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
			addBuck(&(ET[ymin]), &(temp[i]));//insert into ET
		
			}
		}
	}//ET built
	bucket *AL = NULL;
	int yCurr = 0;
	for(int i=0;i<600;i++){//900X600 window---find ymin
		if(ET[i]!=NULL){
			yCurr=i;
			break;
		}
	}
	while(yCurr<maxY){//loop drawing 
		if(ET[yCurr]!=NULL){
			addBuck(&AL, ET[yCurr]);//move into AL
		}
		insertionSort(&AL, yCurr);
		bucket *left = AL;
		bucket *right = AL->link;
		while(right!=NULL){//draw a scanline
			for(int i=left->x;i<right->x;i++){
				C.setPixel(i,yCurr);
			}
			left->x += (left->invM);
			right->x += (right->invM);
			if(right->link==NULL||left->link==NULL){
				break;
			}
			if(right->link==left->link){
				break;
			}
			left = right->link;//go to next pair of edges
			right = left->link;
		}
		yCurr++;
	}
}

