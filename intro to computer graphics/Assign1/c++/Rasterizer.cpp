//
//  Rasterizer.cpp
//
//  Created by Warren R. Carithers on 01/28/14.
//  Based on a C++ version created by Joe Geigel on 11/30/11.
//  Copyright 2014 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//

#include <cmath>

#include "Rasterizer.h"

#include <iostream>
///
//
// Simple class that performs rasterization algorithms
//
///

///
// Constructor
//
// @param n number of scanlines
// @param C the Canvas to be used
///

Rasterizer::Rasterizer( int n, Canvas &canvas ) : n_scanlines(n), C(canvas)
{
}


///
// Draw my initials
//
// Draw my own initials using calls to drawLine(), in the same
// manner that makeLines() in the driver program draws 'CG'.
///

void Rasterizer::myInitials( void ) {

    // ######## Use light blue (0.678,0.847,0.902) for initials ######## 

    C.setColor( 0.678, 0.847, 0.902 );
	drawLine(40,260,260,260);
	drawLine(40,260,40,40);
	drawLine(40,40,260,40);
	drawLine(260,260,260,216);//E
	drawLine(260,40,260,84);
	drawLine(260,128,260,172);
	drawLine(100,128,260,128);
	drawLine(100,172,260,172);
	drawLine(100,84,260,84);
	drawLine(100,216,260,216);
	drawLine(100,84,100,128);
	drawLine(100,172,100,216);
	
	drawLine(340,40,580,40);
	drawLine(340,260,580,260);
	drawLine(340,40,340,84);
	drawLine(580,260,580,216);//S
	drawLine(580,40,580,172);
	drawLine(340,260,340,128);
	drawLine(340,84,520,84);
	drawLine(580,216,400,216);
	drawLine(340,128,520,128);
	drawLine(580,172,400,172);
	drawLine(520,84,520,128);
	drawLine(400,172,400,216);
    //x: 40-260, 340-580
    //y: 340-560...i can use 40-260
    // add code here to draw your initials
    // with calls to your drawLine() function
    // 

}

///
// Draw a line from (x0,y0) to (x1, y1)
//
// Implementation should be using the Midpoint Line Algorithm.
//
// You are to add the implementation here using only calls to the
// setPixel() method of the Canvas.
//
// @param x0 x coord of first endpoint
// @param y0 y coord of first endpoint
// @param x1 x coord of second endpoint
// @param y1 y coord of second endpoint
///

void Rasterizer::drawLine( int x0, int y0, int x1, int y1 )
{
	if(x0==x1){//vertical line
		if(y0>y1){//ensure y1 is higher for looping on y
			int yt=y1;
			y1=y0;
			y0=yt;
		}
		int pixNum = 0;
			for(int i=y0;i<=y1;i++){
				C.setPixel(x0,y0+pixNum);
				pixNum++;
			}
		return;
	}
	if(x1 < x0){
		int xt=x1;//right to left, flip points for looping on x
		int yt=y1;
		x1=x0;
		y1=y0;
		x0=xt;
		y0=yt;
	}
	if(y0==y1){//horizontal
		int pixNum = 0;
			for(int i=x0;i<=x1;i++){
				C.setPixel(x0+pixNum,y0);
				pixNum++;
			}
		return;
	}

	float m = (float)(y1-y0)/ (float)(x1-x0);//calculate slope
		if(m==1||m==-1){//diagonal line
			int pixNum = 0;
			for(int i=x0;i<=x1;i++){
				C.setPixel(x0+pixNum,y0+(pixNum*(int)m));
				pixNum++;
			}
			return;
		}
		if(m>0 && m<1){//gentle positive
			int dE, dNE, x, y, d;
			int dy = y1-y0, dx = x1-x0;
			dE 	= 2*dy;
			dNE = 2*(dy-dx);
			d 	= dE - dx;
			for(x=x0, y=y0; x<=x1; x++){
				C.setPixel(x,y);
				if(d<=0){
					d+=dE;
				}else{
					y++;
					d+= dNE;
				}
			}
			return;
		}
		if(m<0 && m>-1){//gentle negative
			int dE, dSE, x, y, d;
			int dy = y1-y0, dx = x1-x0;
			dE 	= 2*dy;
			dSE = 2*(dy+dx);
			d 	= dE + dx;
			for(x=x0, y=y0; x<=x1; x++){
				C.setPixel(x,y);
				if(d>=0){
					d+=dE;
				}else{
					y--;
					d+= dSE;
				}
			}
			return;
		}
		if(m<-1){//steep neg
		int dS, dSE, x, y, d;
			int dy = y1-y0, dx = x1-x0;
			dS 	= 2*dx;
			dSE = 2*(dy+dx);
			d 	= dy+dS;
			for(x=x0, y=y0; y>=y1; y--){
				C.setPixel(x,y);
				if(d<=0){
					d+=dS;
				}else{
					x++;
					d+= dSE;
				}
			}
			return;
		}
		if(m>1){//steep pos
		int dN, dNE, x, y, d;
			int dy = y1-y0, dx = x1-x0;
			dN 	= -2*dx;
			dNE = 2*(dy-dx);
			d 	= dy+dN;
			for(x=x0, y=y0; y<=y1; y++){
				C.setPixel(x,y);
				if(d>=0){
					d+=dN;
				}else{
					x++;
					d+= dNE;
				}
			}
			return;
		}
    //
    // add code here to implement drawLine()
    // using the midpoint line algorithm
    //

}
