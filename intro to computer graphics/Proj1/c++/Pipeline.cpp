//
//  Pipeline.cpp
//
//  Created by Warren R. Carithers on 2016/10/19.
//  Based on a version created by Joe Geigel on 11/30/11.
//  Copyright 2016 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Erikson Sodergren
//

#include <iostream>

#include "Pipeline.h"
#include "Rasterizer.h"
#include "Clipper.h"
#include <math.h>
///
// Simple wrapper class for midterm assignment
//
// Only methods in the implementation file whose bodies
// contain the comment
//
//    // YOUR IMPLEMENTATION HERE
//
// are to be modified by students.
///
    struct pipeBuck{
		int id;
		int vertCount;
		Vertex *verts;
	};
	
    #define PI 3.14159265
    float trans[3][3];
    int numPolys=0;
    pipeBuck polys[10];
    Rasterizer *rast;
    Clipper clipper;
    Vertex ll, ur;
    float cLeft, cRight, cTop, cBot;//clip window
    float vLeft, vRight, vTop, vBot;//viewport
///
// Constructor
//
// @param w width of canvas
// @param h height of canvas
///
Pipeline::Pipeline( int w, int h ) : Canvas(w,h)
{
    rast = new Rasterizer(h, *this);
    clipper = *(new Clipper());
    ll.x=0;
    ll.y=0;
    ur.x=w;
    ur.y=h;
    clearTransform();
}

void addTrans(float in[3][3]){
	float temp[3][3] = {0};
	for(int i=0;i<3;i++){//column
		for(int j=0;j<3;j++){//row
			for(int k=0;k<3;k++){
				temp[i][j]+= in[i][k]*trans[k][j];
			}
		}
	}
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			trans[i][j] = temp[i][j];
		}
	}
}

void applyTrans(Vertex in[], Vertex out[], int n){
	for(int i=0;i<n;i++){
		float tempX = in[i].x, tempY = in[i].y;
		out[i].x = ((tempX*trans[0][0])+(tempY*trans[0][1])+(trans[0][2]));
	    out[i].y = ((tempX*trans[1][0])+(tempY*trans[1][1])+(trans[1][2]));
	}
}

///
// addPoly - Add a polygon to the canvas.  This method does not draw
//           the polygon, but merely stores it for later drawing.
//           Drawing is initiated by the drawPoly() method.
//
//           Returns a unique integer id for the polygon.
//
// @param p - Array containing the vertices of the polygon to be added.
// @param n - Number of vertices in polygon
//
// @return a unique integer identifier for the polygon
///
int Pipeline::addPoly( const Vertex p[], int n )
{
    Vertex *temp = new Vertex[n];
	for(int i=0;i<n;i++){
		temp[i].x = p[i].x;
		temp[i].y = p[i].y;
	}
	polys[numPolys].id = numPolys;
	polys[numPolys].verts = temp;
	polys[numPolys].vertCount = n;
	numPolys++;
	return numPolys-1;
}

///
// drawPoly - Draw the polygon with the given id.  The polygon should
//            be drawn after applying the current transformation to
//            the vertices of the polygon.
//
// @param polyID - the ID of the polygon to be drawn.
///
void Pipeline::drawPoly( int polyID )
{
    int n = polys[polyID].vertCount;
    //transform
    Vertex tempT[50];
    applyTrans(polys[polyID].verts, tempT, n);
	//clip
	Vertex tempC[50];
	int clipN = clipper.clipPolygon(n, tempT, tempC, ll, ur);
	//viewport
	float xMult = (vRight-vLeft)/(cRight-cLeft);//create conversion vars
	float xAdd  = ((cRight*vLeft)-(cLeft*vRight))/(cRight-cLeft);
	float yMult = (vTop-vBot)/(cTop-cBot);
	float yAdd  = ((cTop*vBot)-(cBot*vTop))/(cTop-cBot);
	for(int i=0;i<clipN;i++){
		float tempX = tempC[i].x, tempY = tempC[i].y;
		tempC[i].x = ((tempX*xMult)+xAdd);
	    tempC[i].y = ((tempY*yMult)+yAdd);
	}//apply it
	//draw
	(*rast).drawPolygon(clipN, tempC);
}

///
// clearTransform - Set the current transformation to the identity matrix.
///
void Pipeline::clearTransform( void )
{
    for(int i=0;i<3;i++){//row
        for(int j=0;j<3;j++){//column
            if(i==j){
                trans[i][i]=1;
            }else{
                trans[i][j]=0;
            }
        }
    }
}

///
// translate - Add a translation to the current transformation by
//             premultiplying the appropriate translation matrix to
//             the current transformation matrix.
//
// @param x - Amount of translation in x.
// @param y - Amount of translation in y.
///
void Pipeline::translate( float tx, float ty )
{
    float t[3][3];
    t[0][0]=1, t[0][1]=0, t[0][2] = tx;
    t[1][0]=0, t[1][1]=1, t[1][2] = ty;
    t[2][0]=0, t[2][1]=0, t[2][2] = 1;
    addTrans(t);
}

///
// rotate - Add a rotation to the current transformation by premultiplying
//          the appropriate rotation matrix to the current transformation
//          matrix.
//
// @param degrees - Amount of rotation in degrees.
///
void Pipeline::rotate( float degrees )
{
	float radians = (PI*degrees)/180;
    float t[3][3];
    t[0][0]=cos(radians), t[0][1]=-1*sin(radians), t[0][2] = 0;
    t[1][0]=sin(radians), t[1][1]=cos(radians), t[1][2] = 0;
    t[2][0]=0, t[2][1]=0, t[2][2] = 1;
	
    addTrans(t);
}

///
// scale - Add a scale to the current transformation by premultiplying
//         the appropriate scaling matrix to the current transformation
//         matrix.
//
// @param x - Amount of scaling in x.
// @param y - Amount of scaling in y.
///
void Pipeline::scale( float sx, float sy )
{
    float t[3][3];
    t[0][0]=sx, t[0][1]=0, t[0][2] = 0;
    t[1][0]=0, t[1][1]=sy, t[1][2] = 0;
    t[2][0]=0, t[2][1]=0, t[2][2] = 1;
    addTrans(t);
}

///
// setClipWindow - Define the clip window.
//
// @param bottom - y coord of bottom edge of clip window (in world coords)
// @param top - y coord of top edge of clip window (in world coords)
// @param left - x coord of left edge of clip window (in world coords)
// @param right - x coord of right edge of clip window (in world coords)
///
void Pipeline::setClipWindow( float bottom, float top, float left, float right )
{
    ll.x=left;
    ll.y=bottom;
    ur.x=right;
    ur.y=top;
    
    cLeft=left;
    cBot=bottom;
    cRight=right;
    cTop=top;
}

///
// setViewport - Define the viewport.
//
// @param xmin - x coord of lower left of view window (in screen coords)
// @param ymin - y coord of lower left of view window (in screen coords)
// @param width - width of view window (in world coords)
// @param height - width of view window (in world coords)
///
void Pipeline::setViewport( int x, int y, int width, int height )
{
    vLeft = x;
    vBot = y;
    vTop = y+height;
    vRight = x+width;
}
