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
	bool horiz;
	int ymin;
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
	bucket bucks[n];
	for(int i=0;i<n;i++){
																					C.setPixel(x[i],y[i]);
		if(i==n-1){
			if(y[i]==y[0]){
				bucks[i].horiz==true;
			}
			if(y[i]<y[0]){
				bucks[i].ymin=y[i];
			}else{
				
			}
		}else{
			
		}
	}
}
