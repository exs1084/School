//
//  textingMain.cpp
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Main program for lighting/shading/texturing assignment
//
//  This file should not be modified by students.
//

#include <cstdlib>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "Buffers.h"
#include "ShaderSetup.h"
#include "Canvas.h"
#include "Shapes.h"
#include "Viewing.h"
#include "Lighting.h"
#include "Textures.h"

using namespace std;

// do we need to do a display() call?
bool updateDisplay = true;

// How to calculate an offset into the vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// our drawing canvas
Canvas *canvas;

// dimensions of the drawing window
int w_width  = 600;
int w_height = 600;
GLfloat lightPos[4] = { 0.5, 0.1, 0.9, 1.0 };
GLfloat lightCol[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat ambient[4]  = { 0.5, 0.5, 0.5, 1.0 };
////////////////pot
GLfloat potmatA[4] = { 0.8, 0.4, 0.4, 1.0 };
GLfloat potmatD[4] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat potmatS[4] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat potkA = 0.5;
GLfloat potkD = 0.9;
GLfloat potkS = 1.0;
GLfloat potspecEx = 5.0;
////////////////cheese
GLfloat cheesematA[4] = { 0.4, 0.4, 0.0, 1.0 };
GLfloat cheesematD[4] = { 0.8, 0.8, 0.0, 1.0 };
GLfloat cheesematS[4] = { 0.8, 0.8, 0.8, 1.0 };

GLfloat cheesekA = 0.8;
GLfloat cheesekD = 0.9;
GLfloat cheesekS = 1.0;
GLfloat cheesespecEx = 30.0;
////////////////plate
GLfloat platematA[4] = { 0.69, 0.25, 0.21, 1.0 };
GLfloat platematD[4] = { 0.33, 0.06, 0.07, 1.0 };
GLfloat platematS[4] = { 0.8, 0.8, 0.8, 1.0 };

GLfloat platekA = 0.8;
GLfloat platekD = 0.9;
GLfloat platekS = 1.0;
GLfloat platespecEx = 200.0;
////////////////wine bottle
GLfloat winematA[4] = { 0.0, 0.26, 0.14, 1.0 };
GLfloat winematD[4] = { 0.0, 0.26, 0.14, 1.0 };
GLfloat winematS[4] = { 0.8, 0.8, 0.8, 1.0 };

GLfloat winekA = 0.8;
GLfloat winekD = 0.9;
GLfloat winekS = 1.0;
GLfloat winespecEx = 30.0;
////////////////grapes
GLfloat grapematA[4] = { 0.89, 0.99, 0.79, 1.0 };
GLfloat grapematD[4] = { 0.89, 0.99, 0.79, 1.0 };
GLfloat grapematS[4] = { 0.8, 0.8, 0.8, 1.0 };

GLfloat grapekA = 0.1;
GLfloat grapekD = 0.7;
GLfloat grapekS = 1.0;
GLfloat grapespecEx = 90.0;
////////////////pears
GLfloat pearmatA[4] = { 0.5, 0.4, 0.0, 1.0 };
GLfloat pearmatD[4] = { 0.9, 0.8, 0.0, 1.0 };
GLfloat pearmatS[4] = { 0.8, 0.8, 0.8, 1.0 };

GLfloat pearkA = 0.5;
GLfloat pearkD = 0.7;
GLfloat pearkS = 1.0;
GLfloat pearspecEx = 90.0;
////////////////knife handle
GLfloat handlematA[4] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat handlematD[4] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat handlematS[4] = { 0.8, 0.8, 0.8, 1.0 };

GLfloat handlekA = 0.7;
GLfloat handlekD = 0.25;
GLfloat handlekS = 0.9;
GLfloat handlespecEx = 3.0;
////////////////knife blade
GLfloat bladematA[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat bladematD[4] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat bladematS[4] = { 0.8, 0.8, 0.8, 1.0 };

GLfloat bladekA = 1.0;
GLfloat bladekD = 0.3;
GLfloat bladekS = 0.0;
GLfloat bladespecEx = 10.0;
//
// We need two vertex buffers and two element buffers:
// one set for the quad (texture mapped), and one set
// for the teapot (Phong shaded)
//
BufferSet quadBuffers;
BufferSet teapotBuffers;
BufferSet cubeBuffers;
BufferSet cylinderBuffers;
BufferSet coneBuffers;
BufferSet sphereBuffers;
BufferSet pearBuffers;

// Animation flag
bool animating = false;

// Initial animation rotation angles
float angles[2] = { 0.0f, 0.0f };

// program IDs...for shader programs
GLuint potshader, tshader;

//
// createShape() - create vertex and element buffers for a shape
//
// @param obj - which shape to create
// @param C   - the Canvas to use
//
void createShape( int obj, Canvas &C )
{
    // clear any previous shape
    C.clear();

    // make the shape
    makeShape( obj, C );

    // create the necessary buffers
    
    switch(obj){
		case OBJ_TEAPOT 	: teapotBuffers.createBuffers( C );
			break;
		case OBJ_QUAD 		: quadBuffers.createBuffers( C );
			break;
		case OBJ_CUBE 		: cubeBuffers.createBuffers( C );
			break;
		case OBJ_CYLINDER 	: cylinderBuffers.createBuffers( C );
			break;
		case OBJ_CONE 		: coneBuffers.createBuffers( C );
			break;
		case OBJ_SPHERE 	: sphereBuffers.createBuffers( C );
			break;
		case OBJ_PEAR		: pearBuffers.createBuffers( C );
			break;
		
	}
}

///
// OpenGL initialization
///
void init( void )
{
    // Create our Canvas
    canvas = new Canvas( w_width, w_height );

    if( canvas == NULL ) {
        cerr << "error - cannot create Canvas" << endl;
        glfwTerminate();
        exit( 1 );
    }

    // Load texture image(s)
    loadTexture();

    // Load shaders, verifying each
    ShaderError error;
    tshader = shaderSetup( "texture.vert", "texture.frag", &error );
    if( !tshader ) {
        cerr << "Error setting up texture shader - " <<
            errorString(error) << endl;
        glfwTerminate();
        exit( 1 );
    }

    potshader = shaderSetup( "phong.vert", "phong.frag", &error );
    if( !potshader ) {
        cerr << "Error setting up Phong shader - " <<
            errorString(error) << endl;
        glfwTerminate();
        exit( 1 );
    }
    

    // Other OpenGL initialization
    glEnable( GL_DEPTH_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );

    // Create all our objects
    createShape( OBJ_QUAD, *canvas );
    createShape( OBJ_CUBE, *canvas );
    createShape( OBJ_TEAPOT, *canvas );
    createShape( OBJ_CYLINDER, *canvas );
    createShape(OBJ_CONE, *canvas);
    createShape(OBJ_SPHERE, *canvas);
    createShape(OBJ_PEAR, *canvas);
    
}

///
// selectBuffers() - bind the correct vertex and element buffers
//
// @param program - GLSL program object
// @param B       - the BufferSet to use
///
void selectBuffers( GLuint program, BufferSet &B ) {

    // bind the buffers
    glBindBuffer( GL_ARRAY_BUFFER, B.vbuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, B.ebuffer );

    // set up the vertex attribute variables
    GLint vPosition = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );
    int offset = B.vSize;

    if( B.cSize ) {  // color data
        GLint vColor = glGetAttribLocation( program, "vColor" );
        glEnableVertexAttribArray( vColor );
        glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B.cSize;
    }

    if( B.nSize ) {  // normal data
        GLint vNormal = glGetAttribLocation( program, "vNormal" );
        glEnableVertexAttribArray( vNormal );
        glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B.nSize;
    }

    if( B.tSize ) {  // texture coordinate data
        GLint vTexCoord = glGetAttribLocation( program, "vTexCoord" );
        glEnableVertexAttribArray( vTexCoord );
        glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B.tSize;
    }
}

///
// Display callback
//
// Invoked whenever the image must be redrawn
///
void display( void )
{
    // clear and draw params..
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // first, the quad
    glUseProgram( tshader );

    // set up viewing and projection parameters
    setUpFrustum( tshader );

    // set up the texture information
    setUpTexture( tshader );

    // set up the camera
    setUpCamera( tshader,
        0.0f, 2.0f, 6.5f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );

    // set up transformations for the table
    setUpTransforms( tshader,
        15.5f, 10.5f, 1.5f,
        90, 0, 0,
        -1.25f, 1.0f, -1.5f
    );

    // draw it
    selectBuffers( tshader, quadBuffers );
    glDrawElements( GL_TRIANGLES, quadBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
        
        //this is the back wall
    setUpTransforms( tshader,
        15.5f, 10.5f, 1.5f,
        0, 0, 0,
        -1.25f, 1.0f, -10.5f
    );
    glDrawElements( GL_TRIANGLES, quadBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
        
    // now, draw the teapot

    glUseProgram( potshader );

    // set up viewing and projection parameters
    setUpFrustum( potshader );

    // set up the Phong shading information
    setPhongProperties(potmatA, potmatD, potmatS, 
potkA, potkD, potkS, potspecEx, lightCol,
lightPos, ambient);
    setUpPhong( potshader );

    // set up the camera
    setUpCamera( potshader,
        0.0f, 2.0f, 6.5f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );

    // draw it
        
    setUpTransforms( potshader,
        .9f, 2.0f, .9f,
        0.0f, -20.0f, 0.0f,
        -1.0f, 0.5f, -1.1f );
    selectBuffers( potshader, teapotBuffers );
    glDrawElements( GL_TRIANGLES, teapotBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
        //THE PLATE
    setPhongProperties(platematA, platematD, platematS, 
platekA, platekD, platekS, platespecEx, lightCol,
lightPos, ambient);
    setUpPhong( potshader );
    setUpTransforms( potshader,
        1.0f, 1.0f, 0.4f,
        -90.0f, 0.0f, 0.0f,
        -1.0f, 0.25f, -0.6f );
    selectBuffers(potshader, coneBuffers);
    glDrawElements( GL_TRIANGLES, coneBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
        //THE CHEEEEESE
    setUpTransforms( potshader,
        0.2f, 0.2f, 0.5f,
        0.0f, 55.0f, 0.0f,
        -1.2f, 0.6f, -0.5f );
    setPhongProperties(cheesematA, cheesematD, cheesematS, 
cheesekA, cheesekD, cheesekS, cheesespecEx, lightCol,
lightPos, ambient);
	setUpPhong(potshader);
    selectBuffers(potshader, cubeBuffers);
    glDrawElements( GL_TRIANGLES, cubeBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
    //WINE BOTTLE TIME
    //BODY FIRST
    setUpTransforms( potshader,
        0.45f, 1.1f, 0.45f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, -2.0f );
    setPhongProperties(winematA, winematD, winematS, 
winekA, winekD, winekS, winespecEx, lightCol,
lightPos, ambient);
	setUpPhong(potshader);
    selectBuffers(potshader, cylinderBuffers);
    glDrawElements( GL_TRIANGLES, cylinderBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
    //NOW CURVE
    setUpTransforms( potshader,
        0.45f, 0.3f, 0.45f,
        90.0f, 0.0f, 0.0f,
        0.0f, 1.75f, -2.0f );
    selectBuffers(potshader, coneBuffers);
    glDrawElements( GL_TRIANGLES, coneBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
    //NOW NECK
    setUpTransforms( potshader,
        0.15f, 1.1f, 0.15f,
        0.0f, 0.0f, 0.0f,
        0.0f, 2.2f, -2.0f );
    selectBuffers(potshader, cylinderBuffers);
    glDrawElements( GL_TRIANGLES, cylinderBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
    //GRAAAAAAAPES x5!
    setUpTransforms( potshader,
        0.1f, 0.1f, 0.1f,
        0.0f, 0.0f, 0.0f,
        -0.7f, 0.35f, 0.5f );
    setPhongProperties(grapematA, grapematD, grapematS, 
grapekA, grapekD, grapekS, grapespecEx, lightCol,
lightPos, ambient);
	setUpPhong(potshader);
	selectBuffers(potshader, sphereBuffers);
    glDrawElements( GL_TRIANGLES, sphereBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );//1
        
    setUpTransforms( potshader,
        0.1f, 0.1f, 0.1f,
        0.0f, 0.0f, 0.0f,
        -0.75f, 0.35f, 0.8f );
    glDrawElements( GL_TRIANGLES, sphereBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );//2
        
    setUpTransforms( potshader,
        0.1f, 0.1f, 0.1f,
        0.0f, 0.0f, 0.0f,
        -1.05f, 0.6f, -.1f );
    glDrawElements( GL_TRIANGLES, sphereBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );//3
        
    setUpTransforms( potshader,
        0.1f, 0.1f, 0.1f,
        0.0f, 0.0f, 0.0f,
        -1.15f, 0.5f, 0.0f );
    glDrawElements( GL_TRIANGLES, sphereBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );//4
        
    setUpTransforms( potshader,
        0.1f, 0.1f, 0.1f,
        0.0f, 0.0f, 0.0f,
        -1.25f, 0.6f, -.1f );
    glDrawElements( GL_TRIANGLES, sphereBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );//5
    //PEARS?!?! 2x!
    setUpTransforms( potshader,
        0.3f, 0.3f, 0.3f,
        0.0f, 0.0f, 0.0f,
        0.25f, 0.7f, -.27f );
    setPhongProperties(pearmatA, pearmatD, pearmatS, 
pearkA, pearkD, pearkS, pearspecEx, lightCol,
lightPos, ambient);
	setUpPhong(potshader);
	selectBuffers(potshader, pearBuffers);
    glDrawElements( GL_TRIANGLES, pearBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );//1
    //this pear needs another piece
    setUpTransforms( potshader,
        0.2f, 0.3f, 0.2f,
        90.0f, 0.0f, 0.0f,
        0.25f, 1.0f, -.26f );
    selectBuffers(potshader, coneBuffers);
    glDrawElements( GL_TRIANGLES, coneBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );//bonus pear
        
    setUpTransforms( potshader,
        0.3f, 0.3f, 0.3f,
        65.0f, 0.0f, 15.0f,
        0.47f, 0.6f, -0.02f );
    selectBuffers(potshader, pearBuffers);
    glDrawElements( GL_TRIANGLES, pearBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );//2
    //KNIFE!
    //HANDLE FIRST!
    setUpTransforms( potshader,
        0.05f, 0.03f, 0.35f,
        10.0f, 45.0f, 0.0f,
        0.6f, 0.32f, 1.0f );
    setPhongProperties(handlematA, handlematD, handlematS, 
handlekA, handlekD, handlekS, handlespecEx, lightCol,
lightPos, ambient);
	setUpPhong(potshader);
    selectBuffers(potshader, cubeBuffers);
    glDrawElements( GL_TRIANGLES, cubeBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
    //NOW THE BLADE!
    setUpTransforms( potshader,
        0.11f, 0.019f, 1.2f,
        10.0f, 45.0f, 0.0f,
        -0.1f, .43f, 0.35f );
    setPhongProperties(bladematA, bladematD, bladematS, 
bladekA, bladekD, bladekS, bladespecEx, lightCol,
lightPos, ambient);
	setUpPhong(potshader);
    selectBuffers(potshader, coneBuffers);
    glDrawElements( GL_TRIANGLES, coneBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
    //GOTTA GET PAPER NOW!
    setUpTransforms( potshader,
        1.3f, 1.3f, 1.3f,
        30.0f, 0.0f, -45.0f,
        0.5f, 1.0f, -0.87f );
    setPhongProperties(bladematA, bladematD, bladematS, 
bladekA, bladekD, bladekS, bladespecEx, lightCol,
lightPos, ambient);
    selectBuffers(potshader, quadBuffers);
    glDrawElements( GL_TRIANGLES, quadBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
    
    setUpTransforms( potshader,
        1.0f, 1.0f, 1.0f,
        65.0f, 25.0f, 0.0f,
        0.45f, 0.73f, -0.27f );
    glDrawElements( GL_TRIANGLES, quadBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
        
    setUpTransforms( potshader,
        1.0f, 1.0f, 1.0f,
        70.0f, -30.0f, 0.0f,
        0.35f, 0.8f, -0.27f );
    glDrawElements( GL_TRIANGLES, quadBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
    
}

///
// Keyboard callback
//
// We don't need to distinguish between (e.g.) 'a' and 'A', so
// we can use the simpler form of callback
///
void keyboard( GLFWwindow *window, int key, int scan, int action, int mods )
{
    if( action != GLFW_PRESS )    // only activate on press, not rpt/release
        return;

    switch( key ) {

        //case GLFW_KEY_A:    // animate
            //animating = true;
            //break;

        //case GLFW_KEY_S:    // stop animating
            //animating = false;
            //break;

        //case GLFW_KEY_R:    // reset rotations
            //angles[0] = 0.0f;
            //angles[1] = 0.0f;
            //break;

        case GLFW_KEY_ESCAPE:   // terminate the program
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose( window, 1 );
            break;
    }

    updateDisplay = true;
}

// Animate the objects (maybe)
void animate( void ) {
    if( animating ) {
        angles[OBJ_QUAD]   += 1;
        angles[OBJ_TEAPOT] += 1;
        updateDisplay = true;
    }
}

void glfwError( int code, const char *desc )
{
    cerr << "GLFW error " << code << ": " << desc << endl;
    exit( 2 );
}

///
// main program for tessellation assignment
///
int main( int argc, char **argv ) {

    glfwSetErrorCallback( glfwError );

    if( !glfwInit() ) {
        cerr << "Can't initialize GLFW!" << endl;
        exit( 1 );
    }

    // NOTE:  Mac users may need to uncomment the following four lines
    // in order to force an OpenGL 3.2 (or better) context

    // glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    // glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
    // glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    // glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    GLFWwindow *window = glfwCreateWindow( w_width, w_height,
        "Final Project", NULL, NULL );

    if( !window ) {
        cerr << "GLFW window create failed!" << endl;
        glfwTerminate();
        exit( 1 );
    }

    glfwMakeContextCurrent( window );

#ifndef __APPLE__
    GLenum err = glewInit();
    if( err != GLEW_OK ) {
        cerr << "GLEW error: " << glewGetErrorString(err) << endl;
        glfwTerminate();
        exit( 1 );
    }
    if( !GLEW_VERSION_3_2 ) {
        cerr << "GLEW: OpenGL 3.2 not available" << endl;
        if( !GLEW_VERSION_2_1 ) {
            cerr << "GLEW: OpenGL 2.1 not available, either!" << endl;
            glfwTerminate();
            exit( 1 );
        }
    }
#endif

    // determine whether or not we can use GLSL 1.50
    int maj = glfwGetWindowAttrib( window, GLFW_CONTEXT_VERSION_MAJOR );
    int min = glfwGetWindowAttrib( window, GLFW_CONTEXT_VERSION_MINOR );

    cerr << "GLFW: using " << maj << "." << min << " context" << endl;
    if( maj < 3 || (maj == 3 && min < 2) ) {
        // nope!
        cerr << "*** GLSL 1.50 shaders may not compile" << endl;
    }

    init();

    glfwSetKeyCallback( window, keyboard );

    while( !glfwWindowShouldClose(window) ) {
        animate();
        if( updateDisplay ) {
            updateDisplay = false;
            display();
            glfwSwapBuffers( window );
        }
        glfwPollEvents();
    }

    glfwDestroyWindow( window );
    glfwTerminate();


    return 0;
}
