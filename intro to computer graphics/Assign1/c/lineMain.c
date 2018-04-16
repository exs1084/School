//
//  lineMain
//
//  Main program for line drawing assignment
//
//  Students should not be modifying this file.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "ShaderSetup.h"
#include "Buffers.h"
#include "Canvas.h"
#include "Rasterizer.h"

// dimensions of drawing window
int w_width  = 600;
int w_height = 600;

// how to calculate an offset into the vertex buffer
#define BUFFER_OFFSET(i)	((char *)NULL + (i))

// our Rasterizer
Rasterizer *rasterizer;

// buffer information
BufferSet shapes;

// shader program ID
GLuint program;

// "fall back" to GLSL 1.20 shaders?
bool useAltShaders = false;

//
// draw all the lines
//
// Idea for lettering style from:
// http://patorjk.com/software/taag/#p=display&f=Star%20Wars&t=Type%20Something
//          _______   ______
//         /  ____|  /  __  \
//        |  |  __  |  |  |  |
//        |  | |_ | |  |  |  |
//        |  |__| | |  `--'  |
//         \______|  \______/
//
void makeLines( Rasterizer *R )
{
    // start with a clean canvas
    canvasClear( rasterizer->C );

    // ######## The letter 'G' in green ########

    rasterizerSetColor( 0.0, 1.0, 0.0, R );

    drawLine(  80, 340, 220, 340, R );  // Horizontal left to right
    drawLine(  40, 380,  80, 340, R );  // 315 degree slope
    drawLine( 220, 340, 260, 380, R );  // 45 degree slope
    drawLine( 260, 380, 260, 440, R );  // Vertical bottom to top
    drawLine( 260, 440, 180, 440, R );  // Horizontal right to left
    drawLine( 180, 440, 180, 400, R );
    drawLine( 180, 400, 220, 400, R );
    drawLine( 220, 400, 200, 380, R );
    drawLine( 200, 380, 100, 380, R );
    drawLine( 100, 380,  80, 400, R );
    drawLine(  80, 400,  80, 500, R );
    drawLine(  80, 500, 100, 520, R );
    drawLine( 100, 520, 200, 520, R );
    drawLine( 200, 520, 220, 500, R );
    drawLine( 220, 500, 220, 480, R );
    drawLine( 220, 480, 260, 480, R );
    drawLine( 260, 480, 260, 520, R );
    drawLine( 260, 520, 220, 560, R );  // 135 degree slope
    drawLine( 220, 560,  80, 560, R );
    drawLine(  80, 560,  40, 520, R );  // 225 degree slope
    drawLine(  40, 520,  40, 380, R );  // Vertical top to bottom

    // ######## The letter 'O' in red ########

    rasterizerSetColor( 1.0, 0.0, 0.0, R );

    drawLine( 450, 320, 520, 340, R );  // 16.6 degree slope
    drawLine( 520, 340, 540, 360, R );  // 45 degree slope
    drawLine( 540, 360, 560, 450, R );  // 77.47 degree slope
    drawLine( 560, 450, 540, 540, R );  // 102.83 degree slope
    drawLine( 540, 540, 520, 560, R );  // 135 degree slope
    drawLine( 520, 560, 450, 580, R );  // 163.3 degree slope
    drawLine( 450, 580, 380, 560, R );  // 196.71 degree slope
    drawLine( 380, 560, 360, 540, R );  // 225 degree slope
    drawLine( 360, 540, 340, 450, R );
    drawLine( 340, 450, 360, 360, R );
    drawLine( 360, 360, 380, 340, R );
    drawLine( 380, 340, 450, 320, R );
    drawLine( 420, 380, 480, 380, R );
    drawLine( 480, 380, 520, 420, R );
    drawLine( 520, 420, 520, 480, R );
    drawLine( 520, 480, 480, 520, R );
    drawLine( 480, 520, 420, 520, R );
    drawLine( 420, 520, 380, 480, R );
    drawLine( 380, 480, 380, 420, R );
    drawLine( 380, 420, 420, 380, R );

    // now, draw the student's initials

    myInitials( R );
}

//
// create the shapes and fill all the buffers
//
void createImage( void )
{
    // draw all our lines
    makeLines( rasterizer );

    createBuffers( &shapes, rasterizer->C );
}


// OpenGL initialization
void init( void )
{
    // Create our Canvas and Rasterizer "objects"
    Canvas *C = canvasCreate( w_width, w_height );
    rasterizer = rasterizerCreate( w_height, C );

    // Load shaders and use the resulting shader program
    ShaderError error;
    if( useAltShaders ) {
        fputs( "Falling back to GLSL 1.20 shaders\n", stderr );
        program = shaderSetup( "alt.vert", "alt.frag", &error );
    } else {
        program = shaderSetup( "shader.vert", "shader.frag", &error );
    }
    if (!program) {
        fprintf( stderr, "Error setting up shaders - %s\n",
            errorString(error) );
	glfwTerminate();
        exit(1);
    }

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );

    // create the geometry for our shapes.
    createImage();
}


//
// selectBuffers() - bind the correct vertex and element buffers
//
// @param program - GLSL program object
// @param B       - the BufferSet to update
//
void selectBuffers( GLuint program, BufferSet *B ) {

    // bind the buffers
    glBindBuffer( GL_ARRAY_BUFFER, B->vbuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, B->ebuffer );

    // set up the vertex attribute variables
    GLuint vPosition = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(B->vSize) );
}


void display( void )
{
    // clear the frame buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // ensure we have selected the correct shader program
    glUseProgram( program );

    // bind our buffers
    selectBuffers( program, &shapes );

    // set up our scale factors for normalization
    GLuint sf = glGetUniformLocation( program, "sf" );
    glUniform2f( sf, 2.0f / (w_width - 1.0f), 2.0f / (w_height - 1.0f) );

    // draw our shape
    glDrawArrays( GL_POINTS, 0, shapes.numElements );
}


void keyboard( GLFWwindow *window, int key, int scan, int action, int mods )
{
    switch( key ) {
        case GLFW_KEY_ESCAPE:
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose( window, 1 );
            break;
    }
}


void glfwError( int code, const char *desc )
{
    fprintf( stderr, "GLFW error %d: %s\n", code, desc );
    exit( 2 );
}


int main( int argc, char **argv )
{
    glfwSetErrorCallback( glfwError );

    if( !glfwInit() ) {
        fputs( "Can't initialize GLFW!\n", stderr );
        exit( 1 );
    }

    GLFWwindow *window = glfwCreateWindow( w_width, w_height,
        "Lab 1 - Line Drawing", NULL, NULL );

    if( !window ) {
        fputs( "GLFW window create failed!\n", stderr );
	glfwTerminate();
        exit( 1 );
    }

    glfwMakeContextCurrent( window );

#ifndef __APPLE__
    GLenum err = glewInit();
    if( err != GLEW_OK ) {
        fprintf( stderr, "GLEW error: %s\n", glewGetErrorString(err) );
	glfwTerminate();
        exit( 1 );
    }

    if( !GLEW_VERSION_3_2 ) {
        fputs( "OpenGL 3.2 not available\n", stderr );
        if( !GLEW_VERSION_2_1 ) {
            fputs( "OpenGL 2.1 not available, either!\n", stderr );
	    glfwTerminate();
            exit( 1 );
	}
    }
#endif

    // determine whether or not we can use GLSL 1.50
    int maj = glfwGetWindowAttrib( window, GLFW_CONTEXT_VERSION_MAJOR );
    int min = glfwGetWindowAttrib( window, GLFW_CONTEXT_VERSION_MINOR );

    fprintf( stderr, "GLFW: using %d.%d context\n", maj, min );
    if( maj < 3 || (maj == 3 && min < 2) ) {
        // nope!
        useAltShaders = true;
    }

    init();

    glfwSetKeyCallback( window, keyboard );

    while( !glfwWindowShouldClose(window) ) {
        display();
        glfwSwapBuffers( window );
        glfwWaitEvents();
    }

    glfwDestroyWindow( window );
    glfwTerminate();

    return 0;
}
