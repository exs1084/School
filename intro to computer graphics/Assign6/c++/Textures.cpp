//
//  Textures
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Contributor:  Erikson Sodergren
//
//  Simple class for setting up texture mapping parameters.
//
//  This code can be compiled as either C or C++.
//

#ifdef __cplusplus
#include <iostream>
#include <stdio.h>
#else
#include <stdio.h>
#endif

#include "Textures.h"

// this is here in case you are using SOIL;
// if you're not, it can be deleted.
#include <SOIL.h>

#ifdef __cplusplus
using namespace std;
#endif

// Add any global definitions and/or variables you need here.
GLuint smiley, frowny;

GLfloat kA2 = 0.5;
GLfloat kD2 = 0.7;
GLfloat kS2 = 1.0;
GLfloat specEx2 = 10.0;

GLfloat lightColor2[] = { 1.0, 1.0, 0.0, 1.0 };
GLfloat lightPos2[] = { 0.0, 5.0, 2.0, 1.0 };

GLfloat ambCol2[] = { 0.5, 0.5, 0.5, 1.0 };

GLuint locs2[10];

///
// This function loads texture data for the GPU.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the various shaders.
///
void loadTexture( void )
{
    // Add your code here.
    smiley = SOIL_load_OGL_texture("smiley2.png", SOIL_LOAD_AUTO, 
    SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | 
    SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    
     if ( smiley == 0 ) { 
       printf( "SOIL loading error: '%s'\n", 
               SOIL_last_result() ); 
}
	frowny = SOIL_load_OGL_texture("frowny2.png", SOIL_LOAD_AUTO, 
    SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | 
    SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    
     if ( frowny == 0 ) { 
       printf( "SOIL loading error: '%s'\n", 
               SOIL_last_result() ); 
}
}

void getLocs2(GLuint program){
	locs2[0] = glGetUniformLocation(program, "smile");
	locs2[1] = glGetUniformLocation(program, "frown");
	locs2[3] = glGetUniformLocation(program, "ka");
	locs2[4] = glGetUniformLocation(program, "kd");
	locs2[5] = glGetUniformLocation(program, "ks");
	locs2[6] = glGetUniformLocation(program, "specExp");
	locs2[7] = glGetUniformLocation(program, "lightColor");
	locs2[8] = glGetUniformLocation(program, "lightPos");
	locs2[9] = glGetUniformLocation(program, "ambient");
}

///
// This function sets up the parameters for texture use.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the various shaders.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpTexture( GLuint program )
{
    // Add your code here.
    getLocs2(program);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, smiley);
    
    glUniform1i(locs2[0], 0);
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, frowny);
    
    glUniform1i(locs2[1], 2);
    glUniform1fv(locs2[3], 1, &kA2);
	glUniform1fv(locs2[4], 1, &kD2);
	glUniform1fv(locs2[5], 1, &kS2);
	glUniform1fv(locs2[6], 1, &specEx2);
	glUniform4fv(locs2[7], 1, lightColor2);
	glUniform4fv(locs2[8], 1, lightPos2);
	glUniform4fv(locs2[9], 1, ambCol2);
	
	
}
