//
//  Buffers.c
//
//  Vertex and element buffer implementations.
//
//  This code can be compiled as either C or C++.
//
//  Students should not be modifying this file.
//

#ifdef __cplusplus
#include <cstdlib>
#include <iostream>
#else
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#endif

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

#include "Buffers.h"
#include "Canvas.h"

#ifdef __cplusplus
using namespace std;
#endif

///
// initBuffer(buf) - reset the supplied BufferSet to its "empty" state
//
// @param buf - which buffer to reset
///
void initBuffer( BufferSet *buf ) {
    buf->vbuffer = buf->ebuffer = 0;
    buf->numElements = 0;
    buf->vSize = buf->eSize = buf->tSize = buf->cSize = 0;
    buf->bufferInit = false;
}

///
// makeBuffer() - make a vertex or element array buffer
//
// @param target - which type of buffer to create
// @param data   - source of data for buffer (or NULL)
// @param size   - desired length of buffer
//
// @return the ID of the new buffer
//
GLuint makeBuffer( GLenum target, const void *data, GLsizei size ) {
    GLuint buffer;

    glGenBuffers( 1, &buffer );
    glBindBuffer( target, buffer );
    glBufferData( target, size, data, GL_STATIC_DRAW );

    return( buffer );
}

///
// createBuffers(buf,canvas) create a set of buffers for the object
//     currently held in 'canvas'.
//
// @param B   - the BufferSet to be modified
// @param C   - the Canvas we'll use for drawing
///
void createBuffers( BufferSet *B, Canvas *C ) {

    // get the vertices
    B->numElements = canvasNumVertices( C );
    float *points = canvasGetVertices( C );
    // #bytes = number of elements * 4 floats/element * bytes/float
    B->vSize = B->numElements * 4 * sizeof(float);
    int vbufSize = B->vSize;

    // get the color data
    float *colors = canvasGetColors( C );
    B->cSize = B->numElements * 4 * sizeof(float);
    vbufSize += B->cSize;

    // get the element data
    GLuint *elements = canvasGetElements( C );
    // #bytes = number of elements * bytes/element
    B->eSize = B->numElements * sizeof(GLuint);

    // set up the vertex buffer
    if( B->bufferInit ) {
        // must delete the existing buffer IDs first
        glDeleteBuffers( 1, &(B->vbuffer) );
        glDeleteBuffers( 1, &(B->ebuffer) );
        initBuffer( B );
    }

    // first, create the connectivity data
    B->ebuffer = makeBuffer( GL_ELEMENT_ARRAY_BUFFER, elements, B->eSize );

    // next, the vertex buffer, containing vertices and "extra" data
    B->vbuffer = makeBuffer( GL_ARRAY_BUFFER, NULL, vbufSize );
    glBufferSubData( GL_ARRAY_BUFFER, 0, B->vSize, points );
    glBufferSubData( GL_ARRAY_BUFFER, B->vSize, B->cSize, colors );

    // NOTE:  'points', 'colors', and 'elements' are dynamically allocated,
    // but we don't free them here because they will be freed at the next
    // call to clear() or the get*() functions

    // finally, mark it as set up
    B->bufferInit = true;
}
