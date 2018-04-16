//
//  Clipper.cpp
//
//  Created by Warren R. Carithers on 01/15/14.
//  Based on a version created by Joe Geigel on 11/30/11.
//  Copyright 2014 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Erikson Sodergren
//

#include "Vertex.h"
#include "Clipper.h"

///
// Simple module that performs clipping
///

///
// Constructor
///
Clipper::Clipper() {
}

bool inside(Vertex v, Vertex bound, int step){//step ==1,2,3,4. 1=right,2=top,3=left,4=bottom
	switch(step){
		case 1 : if(v.x > bound.x){return false;}else{return true;}
		case 2 : if(v.y > bound.y){return false;}else{return true;}
		case 3 : if(v.x < bound.x){return false;}else{return true;}
		case 4 : if(v.y < bound.y){return false;}else{return true;}
	}
}

Vertex intersect(Vertex p, Vertex s, Vertex b1, int step){//x==true:vertical boundary, fasle=horizontal
	Vertex ret = {0,0};
	float m = (float)(p.y-s.y)/(p.x-s.x);
	bool x = true;
	if(step==2||step==4){
		x = false;
	}
	if(x){
		ret.x = b1.x;
		int dX = ret.x-s.x;
		int retY = s.y + (m*dX);
		ret.y=retY;
	}else{
		ret.y=b1.y;
		int dY = ret.y-s.y;
		int retX = s.x + (dY/m);
		ret.x=retX;
	}
	return ret;
}

int SHPC(Vertex inV[], Vertex outV[], int in, Vertex bound, int step){
	int out=0;
	Vertex p = inV[in-1];
	Vertex s = {0,0};
	
	for(int j=0;j<in;j++){
		s = inV[j];
		if(inside(s, bound, step)){
			if(inside(p, bound, step)){
				outV[out]=s;
				out++;
			}else{
				outV[out] = intersect(p, s, bound, step);
				out++;
				outV[out] = s;
				out++;
			}
		}else{
			if(inside(p, bound, step)){
				outV[out] = intersect(p, s, bound, step);
				out++;
			}
		}
		p = s;
	}
	
	return out;
}

///
// clipPolygon
//
// Clip the polygon with vertex count in and vertices inV against the
// rectangular clipping region specified by lower-left corner ll and
// upper-right corner ur. The resulting vertices are placed in outV.
//
// The routine should return the with the vertex count of polygon
// resulting from the clipping.
//
// @param in    the number of vertices in the polygon to be clipped
// @param inV   the incoming vertex list
// @param outV  the outgoing vertex list
// @param ll    the lower-left corner of the clipping rectangle
// @param ur    the upper-right corner of the clipping rectangle
//
// @return number of vertices in the polygon resulting after clipping
//
///

int Clipper::clipPolygon( int in, const Vertex inV[], Vertex outV[],
                          Vertex ll, Vertex ur )
{
    // YOUR CODE GOES HERE
	int count = 0;
	Vertex ul = {ll.x, ur.y};
	Vertex lr = {ur.x, ll.y};
	Vertex inputs[50];
	for(int i=0;i<in;i++){
		inputs[i] = inV[i];
	}
	Vertex temp1[50];
	Vertex temp2[50];
	Vertex temp3[50];
	int in2, in3, in4;
	
	in2 = SHPC(inputs, temp1, in, ur,1);
	in3 = SHPC(temp1, temp2, in2, ur,2);
	in4 = SHPC(temp2, temp3, in3, ll,3);
	count = SHPC(temp3, outV, in4, ll,4);

    return( count );  // remember to return the outgoing vertex count!

}
