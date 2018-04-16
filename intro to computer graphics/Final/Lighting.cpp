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

GLfloat matA[4] = { 0.5, 0.1, 0.9, 1.0 };
GLfloat matD[4] = { 0.89, 0.0, 0.0, 1.0 };
GLfloat matS[4] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat kA = 0.5;
GLfloat kD = 0.7;
GLfloat kS = 1.0;
GLfloat specEx = 10.0;

GLfloat lightColor1[4] = { 1.0, 1.0, 0.0, 1.0 };
GLfloat lightPos1[4] = { 15.0, 5.0, 2.0, 1.0 };

GLfloat ambCol[4] = { 0.5, 0.5, 0.5, 1.0 };

GLuint locs[10];

void setPhongProperties(GLfloat amb[4], GLfloat diffuse[4], GLfloat specular[4], 
GLfloat Ka, GLfloat Kd, GLfloat Ks, GLfloat specExponent, GLfloat light1Col[4],
GLfloat light1Pos[4], GLfloat ambientCol[4]){
	for(int i=0;i<4;i++){
		matA[i]=amb[i], matD[i]=diffuse[i], matS[i]=specular[i];
		lightColor1[i] = light1Col[i], lightPos1[i]=light1Pos[i];
		ambCol[i] = ambientCol[i];
	}
	kA=Ka, kD=Kd, kS=Ks, specEx = specExponent;
}

void getLocs(GLuint program){
	locs[0] = glGetUniformLocation(program, "Oa");
	locs[1] = glGetUniformLocation(program, "Od");
	locs[2] = glGetUniformLocation(program, "Os");
	locs[3] = glGetUniformLocation(program, "ka");
	locs[4] = glGetUniformLocation(program, "kd");
	locs[5] = glGetUniformLocation(program, "ks");
	locs[6] = glGetUniformLocation(program, "specExp");
	locs[7] = glGetUniformLocation(program, "lightColor1");
	locs[8] = glGetUniformLocation(program, "lightPos1");
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
	glUniform4fv(locs[7], 1, lightColor1);
	glUniform4fv(locs[8], 1, lightPos1);
	glUniform4fv(locs[9], 1, ambCol);
}
