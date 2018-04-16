//
//  Canvas.c
//
//  Routines for adding points to create a new mesh.
//
//  Students are not to modify this file.
//

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

//
// GLEW and GLFW header files also pull in the OpenGL definitions
//

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Canvas.h"
#include "FloatVector.h"

///
// Constructor
//
// @param w width of canvas
// @param h height of canvas
///
Canvas *canvasCreate( int w, int h ) {
    Canvas *C = calloc( 1, sizeof(Canvas) );
    if( C != NULL ) {
        C->width = w;
        C->height = h;
        // all other fields contain zeroes
    }
    return( C );
}


///
// Destructor
//
// @param C canvas to destroy
///
void canvasDestroy( Canvas *C ) {
    if( !C )
        return;
    free( C );
}


///
// clear the canvas
//
// @param C The Canvas to use
///
void canvasClear( Canvas *C )
{
    if( C->pointArray ) {
        free( C->pointArray );
        C->pointArray = 0;
    }
    if( C->elemArray ) {
        free( C->elemArray );
        C->elemArray = 0;
    }
    if( C->colorArray ) {
        free( C->colorArray );
        C->colorArray = 0;
    }
    fvClear( &(C->points) );
    fvClear( &(C->colors) );
    C->numElements = 0;
    C->currentColor[0] = 0.0;
    C->currentColor[1] = 0.0;
    C->currentColor[2] = 0.0;
}


///
// change the current drawing color
//
// @param r The red component of the new color (between 0-1)
// @param g The green component of the new color (between 0-1)
// @param b The blue component of the new color (between 0-1);
// @param C The Canvas to use
///
void canvasSetColor( float r, float g, float b, Canvas *C )
{
    C->currentColor[0] = r;
    C->currentColor[1] = g;
    C->currentColor[2] = b;
}


///
// set a pixel to the current drawing color
//
// @param x The x coord of the pixel to be set
// @param y The y coord of the pixel to be set
// @param C The Canvas to use
///
void canvasSetPixel( int x0, int y0, Canvas *C )
{
    fvPushBack( &(C->points), (float) x0 );
    fvPushBack( &(C->points), (float) y0 );
    fvPushBack( &(C->points), -1.0 );  // fixed Z depth
    fvPushBack( &(C->points), 1.0 );

    fvPushBack( &(C->colors), C->currentColor[0] );
    fvPushBack( &(C->colors), C->currentColor[1] );
    fvPushBack( &(C->colors), C->currentColor[2] );
    fvPushBack( &(C->colors), 1.0 );   // alpha channel

    C->numElements += 1;
}


///
// gets the array of vertices for the current shape
//
// @param C The Canvas to use
///
float *canvasGetVertices( Canvas *C )
{
    int i;

    // delete the old point array if we have one
    if( C->pointArray ) {
        free( C->pointArray );
    }

    // create and fill a new point array
    C->pointArray = (float *) malloc(
        fvSize(&(C->points)) * sizeof(float) );
    if( C->pointArray == 0 ) {
    	perror( "point allocation failure" );
	exit( 1 );
    }
    for( i = 0; i < fvSize(&(C->points)); i++ ) {
        C->pointArray[i] = C->points.vec[i];
    }

    return C->pointArray;
}


///
// gets the array of elements for the current shape
//
// @param C The Canvas to use
///
GLuint *canvasGetElements( Canvas *C )
{
    int i;

    // delete the old element array if we have one
    if( C->elemArray ) {
        free( C->elemArray );
    }

    // create and fill a new element array
    C->elemArray = (GLuint *) malloc(
        fvSize(&(C->points)) * sizeof(GLuint) );
    if( C->elemArray == 0 ) {
    	perror( "element allocation failure" );
	exit( 1 );
    }
    for( i = 0; i < fvSize(&(C->points)); i++ ) {
        C->elemArray[i] = i;
    }

    return C->elemArray;
}


///
// gets the array of colors for the current shape
//
// @param C The Canvas to use
///
float *canvasGetColors( Canvas *C )
{
    int i;

    // delete the old color array if we have one
    if( C->colorArray ) {
        free( C->colorArray );
    }

    // create and fill a new color array
    C->colorArray = (float *) malloc(
        fvSize(&(C->colors)) * sizeof(float) );
    if( C->colorArray == 0 ) {
    	perror( "color allocation failure" );
	exit( 1 );
    }
    for( i = 0; i < fvSize(&(C->colors)); i++ ) {
        C->colorArray[i] = C->colors.vec[i];
    }

    return C->colorArray;
}


///
// returns number of vertices in current shape
//
// @param C The Canvas to use
///
int canvasNumVertices( Canvas *C )
{
    return C->numElements;
}
