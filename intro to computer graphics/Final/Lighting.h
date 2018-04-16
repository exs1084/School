//
//  Lighting.h
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Simple class for setting up Phong illumination/shading.
//
//  This file should not be modified by students.
//

#ifndef _LIGHTING_H_
#define _LIGHTING_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

///
// This function sets up the lighting, material, and shading parameters
// for the Phong shader.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///

void setPhongProperties(GLfloat amb[4], GLfloat diffuse[4], GLfloat specular[4], 
GLfloat Ka, GLfloat Kd, GLfloat Ks, GLfloat specExponent, GLfloat light1Col[4],
GLfloat light1Pos[4], GLfloat ambientCol[4]);

void setUpPhong( GLuint program );

#endif
