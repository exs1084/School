//
//  Buffers.h
//
//  Vertex and element buffer management
//
//  Author:  Warren R. Carithers
//  Date:    2016/10/11 12:34:54
//

#ifndef _BUFFERS_H_
#define _BUFFERS_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "Canvas.h"

///
// All the relevant information needed to keep
// track of vertex and element buffers
///

typedef struct BufSet {
    GLuint vbuffer, ebuffer;           // buffer handles
    int numElements;                   // total number of vertices
    long vSize, eSize, tSize, cSize;   // component sizes (bytes)
    bool bufferInit;                   // have these already been set up?
} BufferSet;

///
// Support functions
///

///
// initBuffer(buf) - reset the supplied buffer to its "empty" state
//
// @param buf - which buffer to reset
///
void initBuffer( BufferSet *buf );

///
// makeBuffer(target,data,size) - make a vertex or element array buffer
//
// @param target - which type of buffer to create
// @param data   - source of data for buffer (or NULL)
// @param size   - desired length of buffer
//
// @return the ID of the new buffer
///
GLuint makeBuffer( GLenum target, const void *data, GLsizei size );

///
// createBuffers(buf,canvas) - create a set of buffers for the object
//     currently held in 'canvas'.
//
// @param B   - the BufferSet to be modified
// @param C   - the Canvas we'll use for drawing
///
void createBuffers( BufferSet *B, Canvas *C );

#endif
