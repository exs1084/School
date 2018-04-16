//
//  Rasterizer.h
//
//  Created by Warren R. Carithers on 01/27/14.
//  Based on a C++ version created by Joe Geigel on 11/30/11.
//  Copyright 2014 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//

#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#include "Canvas.h"

///
// Simple structure that performs rasterization
///

typedef struct st_Rasterizer {

    ///
    // number of scanlines
    ///

    int n_scanlines;

    ///
    // Drawing canvas
    ///

    Canvas *C;

} Rasterizer;

///
// Constructor
//
// @param n number of scanlines
// @param C drawing canvas to use
///
Rasterizer *rasterizerCreate( int n, Canvas *C );

///
// Destructor
//
// @param R Rasterizer to destroy
///
void rasterizerDestroy( Rasterizer *R );

///
// Set the canvas drawing color
//
// @param r The red component of the new color (between 0-1)
// @param g The green component of the new color (between 0-1)
// @param b The blue component of the new color (between 0-1)
///
void rasterizerSetColor( float r, float g, float b, Rasterizer *R );

///
// Draw my initials
//
// Draw my own initials using calls to drawLine(), in the same
// manner that makeLines() in the driver program draws 'GO'.
//
// @param R the Rasterizer to be used
//
///
void myInitials( Rasterizer *R );

///
// Draw a line from (x0,y0) to (x1, y1)
//
// Implementation should be using the Midpoint Line Algorithm.
//
// You are to add the implementation here using only calls to the
// canvasSetPixel() function.
//
// @param x0 x coord of first endpoint
// @param y0 y coord of first endpoint
// @param x1 x coord of second endpoint
// @param y1 y coord of second endpoint
// @param R  the relevant Rasterizer to draw with
///
void drawLine( int x0, int y0, int x1, int y1, Rasterizer *R );

#endif
