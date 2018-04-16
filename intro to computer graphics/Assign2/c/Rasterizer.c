//
//  Rasterizer.c
//
//  Created by Warren R. Carithers on 01/28/14.
//  Based on a C++ version created by Joe Geigel on 11/30/11.
//  Copyright 2014 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//

#include <stdlib.h>

#include "Rasterizer.h"
#include "Canvas.h"

///
// Simple class that performs rasterization algorithms
///

///
// Constructor
//
// @param n number of scanlines
// @param C The Canvas to use
///
Rasterizer *rasterizerCreate( int n, Canvas *C )
{
    Rasterizer *new = calloc( 1, sizeof(Rasterizer) );
    if( new != NULL ) {
        new->n_scanlines = n;
	new->C = C;
    }
    return( new );
}

///
// Destructor
//
// @param R Rasterizer to destroy
///
void rasterizerDestroy( Rasterizer *R )
{
    if( R )
        free( R );
}

///
// Set the canvas drawing color
//
// @param r The red component of the new color (between 0-1)
// @param g The green component of the new color (between 0-1)
// @param b The blue component of the new color (between 0-1)
// @param R The Rasterizer to use
///
void rasterizerSetColor( float r, float g, float b, Rasterizer *R ) {
    canvasSetColor( r, g, b, R->C );
}

///
// Draw a filled polygon
//
// Implementation should use the scan-line polygon fill algorithm
// discussed in class.
//
// The polygon has n distinct vertices.  The coordinates of the vertices
// making up the polygon are stored in the x and y arrays.  The ith
// vertex will have coordinate (x[i],y[i]).
//
// You are to add the implementation here using only calls to the
// canvasSetPixel() function.
//
// @param n - number of vertices
// @param x - array of x coordinates
// @param y - array of y coordinates
// @param R - The Rasterizer to be used
///
void drawPolygon( int n, const int x[], const int y[], Rasterizer *R )
{
    // YOUR IMPLEMENTATION GOES HERE

}
