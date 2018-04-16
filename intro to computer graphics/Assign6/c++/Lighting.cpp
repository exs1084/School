//
//  Lighting
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Contributor:  Erikson Sodergren
//
//  Simple class for setting up Phong illumination/shading.
//
//  This code can be compiled as either C or C++.
//

#include "Lighting.h"

// Add any global definitions and/or variables you need here.

///
// This function sets up the lighting, material, and shading parameters
// for the Phong shader.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///

GLfloat matA[] = { 0.5, 0.1, 0.9, 1.0 };
GLfloat matD[] = { 0.89, 0.0, 0.0, 1.0 };
GLfloat matS[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat kA = 0.5;
GLfloat kD = 0.7;
GLfloat kS = 1.0;
GLfloat specEx = 10.0;

GLfloat lightColor[] = { 1.0, 1.0, 0.0, 1.0 };
GLfloat lightPos[] = { 0.0, 5.0, 2.0, 1.0 };

GLfloat ambCol[] = { 0.5, 0.5, 0.5, 1.0 };

GLuint locs[10];

void getLocs(GLuint program){
	locs[0] = glGetUniformLocation(program, "Oa");
	locs[1] = glGetUniformLocation(program, "Od");
	locs[2] = glGetUniformLocation(program, "Os");
	locs[3] = glGetUniformLocation(program, "ka");
	locs[4] = glGetUniformLocation(program, "kd");
	locs[5] = glGetUniformLocation(program, "ks");
	locs[6] = glGetUniformLocation(program, "specExp");
	locs[7] = glGetUniformLocation(program, "lightColor");
	locs[8] = glGetUniformLocation(program, "lightPos");
	locs[9] = glGetUniformLocation(program, "ambient");
}

void setUpPhong( GLuint program )
{
    // Add your code here
    getLocs(program);
	glUniform4fv(locs[0], 1, matA);
	glUniform4fv(locs[1], 1, matD);
	glUniform4fv(locs[2], 1, matS);
	glUniform1fv(locs[3], 1, &kA);
	glUniform1fv(locs[4], 1, &kD);
	glUniform1fv(locs[5], 1, &kS);
	glUniform1fv(locs[6], 1, &specEx);
	glUniform4fv(locs[7], 1, lightColor);
	glUniform4fv(locs[8], 1, lightPos);
	glUniform4fv(locs[9], 1, ambCol);
}
