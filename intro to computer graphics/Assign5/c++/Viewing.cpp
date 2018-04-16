//
//  Viewing
//
//  Created by Warren R. Carithers 2016/11/11.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Simple class for setting up the viewing and projection transforms
//  for the Transformation Assignment.
//
//  This code can be compiled as either C or C++.
//
//  Contributor:  Erikson Sodergren
//

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "Viewing.h"
#include <cstdlib>
#include <iostream>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
//globals
GLuint rLoc, sLoc, tLoc, eyeLoc, lookLoc, upLoc, projLoc1, projLoc2;
GLfloat angles[3] = {0.0f, 0.0f, 0.0f};
GLfloat scales[3] = {1.0f, 1.0f, 1.0f};
GLfloat trans[3]  = {0.0f, 0.0f, 0.0f};

GLfloat eyes[3]	= {0.0f, 0.0f, 0.0f};
GLfloat look[3]	= {0.0f, 0.0f, -1.0f};
GLfloat upvals[3] = {0.0f, 1.0f, 0.0f};

GLfloat proj1[3], proj2[3];
GLfloat projBounds[6] = {-1.0, 1.0, 0.9, -1.0, 1.0, 4.5};


void globalSetup(GLuint program){
	rLoc = glGetUniformLocation(program, "theta");
	sLoc = glGetUniformLocation(program, "scales");
	tLoc = glGetUniformLocation(program, "trans");
	
	eyeLoc = glGetUniformLocation(program, "eye");
	lookLoc = glGetUniformLocation(program, "lookat");
	upLoc = glGetUniformLocation(program, "up");
	
	projLoc1 = glGetUniformLocation(program, "proj1");
	projLoc2 = glGetUniformLocation(program, "proj2");
}
///
// This function sets up the view and projection parameter for a frustum
// projection of the scene. See the assignment description for the values
// for the projection parameters.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpFrustum( GLuint program )
{
    // Add your code here.
    GLuint boolLoc = glGetUniformLocation(program, "ortho");
    glUniform1i(boolLoc, false);//tell shader to use frustum
    for(int i=0;i<3;i++){
		proj1[i] = projBounds[i];
		proj2[i] = projBounds[i+3];//make sure bounds are given
	}
	globalSetup(program);
	glUniform3fv(projLoc1, 1, proj1);
	glUniform3fv(projLoc2, 1, proj2);
}


///
// This function sets up the view and projection parameter for an
// orthographic projection of the scene. See the assignment description
// for the values for the projection parameters.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpOrtho( GLuint program )
{
    // Add your code here.
    GLuint boolLoc = glGetUniformLocation(program, "ortho");
    glUniform1i(boolLoc, true);//tell shader use ortho
    for(int i=0;i<3;i++){
		proj1[i] = projBounds[i];//make sure bounds are given
		proj2[i] = projBounds[i+3];
	}
	glUniform3fv(projLoc1, 1, proj1);
	glUniform3fv(projLoc2, 1, proj2);
    globalSetup(program);
}


///
// This function clears any model transformations, setting the values
// to the defaults: no scaling (scale factor of 1), no rotation (degree
// of rotation = 0), and no translation (0 translation in each direction).
//
// You will need to write this function, and maintain all of the values
// which must be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void clearTransforms( GLuint program )
{
    // Add your code here.
    globalSetup(program);
	for(int i=0;i<3;i++){
		angles[i]=0.0f;
		scales[i]=1.0f;
		trans[i]=0.0f;
	}
	glUniform3fv( rLoc, 1, angles );
	glUniform3fv( sLoc, 1, scales );
	glUniform3fv( tLoc, 1, trans );
}


///
// This function sets up the transformation parameters for the vertices
// of the teapot.  The order of application is specified in the driver
// program.
//
// You will need to write this function, and maintain all of the values
// which must be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param scaleX - amount of scaling along the x-axis
// @param scaleY - amount of scaling along the y-axis
// @param scaleZ - amount of scaling along the z-axis
// @param rotateX - angle of rotation around the x-axis, in degrees
// @param rotateY - angle of rotation around the y-axis, in degrees
// @param rotateZ - angle of rotation around the z-axis, in degrees
// @param translateX - amount of translation along the x axis
// @param translateY - amount of translation along the y axis
// @param translateZ - amount of translation along the z axis
///
void setUpTransforms( GLuint program,
    GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ,
    GLfloat rotateX, GLfloat rotateY, GLfloat rotateZ,
    GLfloat translateX, GLfloat translateY, GLfloat translateZ )
{
    // Add your code here.
    globalSetup(program);
    angles[0]= rotateX;
    angles[1]= rotateY;
    angles[2]= rotateZ;
    scales[0]= scaleX;
    scales[1]= scaleY;
    scales[2]= scaleZ;
    trans[0] = translateX;
    trans[1] = translateY;
    trans[2] = translateZ;
    
    
    glUniform3fv( rLoc, 1, angles );
	glUniform3fv( sLoc, 1, scales );
	glUniform3fv( tLoc, 1, trans );
}


///
// This function clears any changes made to camera parameters, setting the
// values to the defaults: eye (0.0,0.0,0.0), lookat (0,0,0.0,-1.0),
// and up vector (0.0,1.0,0.0).
//
// You will need to write this function, and maintain all of the values
// which must be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void clearCamera( GLuint program )
{
    // Add your code here.
    eyes[0]= 0.0f;
    eyes[1]= 0.0f;
    eyes[2]= 0.0f;
    look[0]= 0.0f;
    look[1]= 0.0f;
    look[2]= -1.0f;
    upvals[0] = 0.0f;
    upvals[1] = 1.0f;
    upvals[2] = 0.0f;
    
    glUniform3fv( eyeLoc, 1, eyes );
	glUniform3fv( lookLoc, 1, look );
	glUniform3fv( upLoc, 1, upvals );
}

///
// This function sets up the camera parameters controlling the viewing
// transformation.
//
// You will need to write this function, and maintain all of the values
// which must be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param eyeX - x coordinate of the camera location
// @param eyeY - y coordinate of the camera location
// @param eyeZ - z coordinate of the camera location
// @param lookatX - x coordinate of the lookat point
// @param lookatY - y coordinate of the lookat point
// @param lookatZ - z coordinate of the lookat point
// @param upX - x coordinate of the up vector
// @param upY - y coordinate of the up vector
// @param upZ - z coordinate of the up vector
///
void setUpCamera( GLuint program,
    GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ,
    GLfloat lookatX, GLfloat lookatY, GLfloat lookatZ,
    GLfloat upX, GLfloat upY, GLfloat upZ )
{
    // Add your code here.
    globalSetup(program);
    eyes[0]= eyeX;
    eyes[1]= eyeY;
    eyes[2]= eyeZ;
    look[0]= lookatX;
    look[1]= lookatY;
    look[2]= lookatZ;
    upvals[0] = upX;
    upvals[1] = upY;
    upvals[2] = upZ;
    
    glUniform3fv( eyeLoc, 1, eyes );
	glUniform3fv( lookLoc, 1, look );
	glUniform3fv( upLoc, 1, upvals );
}
