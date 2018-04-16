//
//  Shapes
//
//  Routines for tessellating a number of basic shapes
//
//  Students are to supply their implementations for the functions in
//  this file using the function addTriangle() to do the tessellation.
//
//  This code can be compiled as either C or C++.
//
//
//	Erikson Sodergren

#include <cmath>
#include <iostream>

// Canvas.h pulls in all the OpenGL/GLFW/etc. header files for us
#include "Shapes.h"
#include "Canvas.h"
#include "Vertex.h"

#define PI 3.14159265

///
// makeCube - Create a unit cube, centered at the origin, with a given number
// of subdivisions in each direction on each face.
//
// @param subdivision - number of equal subdivisions to be made in each 
//        direction along each face
//
// Can only use calls to C.addTriangle()
///
void makeCube( Canvas &C, int subdivisions )
{
    if( subdivisions < 1 )
        subdivisions = 1;

    // YOUR IMPLEMENTATION HERE
    float Q[subdivisions+1];
    Vertex P[subdivisions+1][subdivisions+1];
    float u=0,v=0;
    //first face: z=0.5
    for(int i=0;i<=subdivisions;i++){//create vertices
		u=((float)i)/subdivisions;
		Q[i]=((1-u)*-.5)+u*.5;//determine x=values
		for(int j=0;j<=subdivisions;j++){//y values
			v=((float)j)/subdivisions;
			P[i][j].z=0.5;
			P[i][j].y=Q[i];
			P[i][j].x=((1-v)*-.5)+v*.5;
		}
	}
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangle(P[i][j], P[i][j+1], P[i+1][j+1]);//upper triangle
			C.addTriangle(P[i+1][j+1], P[i+1][j], P[i][j]);//lower triangle
		}
	}
	
	//second face: z=-0.5
    for(int i=0;i<=subdivisions;i++){//create vertices
		u=((float)i)/subdivisions;
		Q[i]=((1-u)*.5)+u*-.5;//determine x=values
		for(int j=0;j<=subdivisions;j++){//y values
			v=((float)j)/subdivisions;
			P[i][j].z=-0.5;
			P[i][j].y=Q[i];
			P[i][j].x=((1-v)*-.5)+v*.5;
		}
	}
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangle(P[i][j], P[i][j+1], P[i+1][j+1]);//upper triangle
			C.addTriangle(P[i+1][j+1], P[i+1][j], P[i][j]);//lower triangle
		}
	}
	
	//third face: x=0.5--needs work
    for(int i=0;i<=subdivisions;i++){//create vertices
		u=((float)i)/subdivisions;
		Q[i]=((1-u)*-.5)+u*.5;//determine y=values
		for(int j=0;j<=subdivisions;j++){//z values
			v=((float)j)/subdivisions;
			P[i][j].x=0.5;
			P[i][j].z=Q[i];
			P[i][j].y=((1-v)*-.5)+v*.5;
		}
	}
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangle(P[i][j], P[i][j+1], P[i+1][j+1]);//upper triangle
			C.addTriangle(P[i+1][j+1], P[i+1][j], P[i][j]);//lower triangle
		}
	}
	
	//fourth face: x=-0.5
    for(int i=0;i<=subdivisions;i++){//create vertices
		u=((float)i)/subdivisions;
		Q[i]=((1-u)*.5)+u*-.5;//determine y=values
		for(int j=0;j<=subdivisions;j++){//z values
			v=((float)j)/subdivisions;
			P[i][j].x=-0.5;
			P[i][j].z=Q[i];
			P[i][j].y=((1-v)*-.5)+v*.5;
		}
	}
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangle(P[i][j], P[i][j+1], P[i+1][j+1]);//upper triangle
			C.addTriangle(P[i+1][j+1], P[i+1][j], P[i][j]);//lower triangle
		}
	}
	
	//fifth face: y=0.5--needs work
    for(int i=0;i<=subdivisions;i++){//create vertices
		u=((float)i)/subdivisions;
		Q[(int)(u*subdivisions)]=((1-u)*-.5)+u*.5;//determine z=values
		for(int j=0;j<=subdivisions;j++){//x values
			v=((float)j)/subdivisions;
			P[i][j].y=0.5;
			P[i][j].x=Q[i];
			P[i][j].z=((1-v)*-.5)+v*.5;
		}
	}
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangle(P[i][j], P[i][j+1], P[i+1][j]);//upper triangle
			C.addTriangle(P[i][j+1], P[i+1][j+1], P[i+1][j]);//lower triangle
		}
	}
	
	//sixth face: y=-0.5
    for(int i=0;i<=subdivisions;i++){//create vertices
		u=((float)i)/subdivisions;
		Q[(int)(u*subdivisions)]=((1-u)*.5)+u*-.5;//determine z=values
		for(int j=0;j<=subdivisions;j++){//x values
			v=((float)j)/subdivisions;
			P[i][j].y=-0.5;
			P[i][j].x=Q[i];
			P[i][j].z=((1-v)*-.5)+v*.5;
		}
	}
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangle(P[i][j], P[i][j+1], P[i+1][j]);//upper triangle
			C.addTriangle(P[i][j+1], P[i+1][j+1], P[i+1][j]);//lower triangle
		}
	}
}


///
// makeCylinder - Create polygons for a cylinder with unit height, centered at
// the origin, with separate number of radial subdivisions and height 
// subdivisions.
//
// @param radius - Radius of the base of the cylinder
// @param radialDivision - number of subdivisions on the radial base
// @param heightDivisions - number of subdivisions along the height
//
// Can only use calls to C.addTriangle()
///
void makeCylinder( Canvas &C, float radius, int radialDivisions, int heightDivisions )
{
    if( radialDivisions < 3 )
        radialDivisions = 3;

    if( heightDivisions < 1 )
        heightDivisions = 1;

    // YOUR IMPLEMENTATION HERE
    float discZ[radialDivisions], discX[radialDivisions];
    //disc math
    double alpha=0;
    Vertex top[radialDivisions+1], bot[radialDivisions+1];
    top[radialDivisions].y=0.5;
    top[radialDivisions].x=0;//centerpoints for drawing discs
    top[radialDivisions].z=0;
    
    bot[radialDivisions].y=-.5;
    bot[radialDivisions].x=0;
    bot[radialDivisions].z=0;
    for(int i=0;i<radialDivisions;i++){//circumference of discs
		discX[i]=radius*sin(alpha);
		discZ[i]=radius*cos(alpha);
		top[i].x=discX[i];
		top[i].y=0.5;
		top[i].z=discZ[i];
		
		bot[i].x=discX[i];
		bot[i].y=-.5;
		bot[i].z=discZ[i];
		alpha+=2*PI/((double)radialDivisions);
	}
	//draw discs
	for(int i=0;i<radialDivisions;i++){
		if(i==radialDivisions-1){
			C.addTriangle(top[radialDivisions], top[i], top[0]);
			C.addTriangle(bot[radialDivisions], bot[0], bot[i]);
		}else{
			C.addTriangle(top[radialDivisions], top[i], top[i+1]);
			C.addTriangle(bot[radialDivisions], bot[i+1], bot[i]);
		}
	}
	
	//faces
	Vertex P1, P2;
	for(int i=0;i<radialDivisions;i++){//loop faces
		Vertex left[heightDivisions+1];
		Vertex right[heightDivisions+1];
		if(i==radialDivisions-1){//last face
			P1=bot[i], P2=bot[0];
		}else{
			P1=bot[i], P2=bot[i+1];
		}
		left[0]=P1;
		right[0]=P2;
		float yCurr=-0.5;
		for(int j=1;j<=heightDivisions;j++){//loop divisions
			yCurr+=1.0/((float)heightDivisions);
			left[j].y=yCurr;
			left[j].x=P1.x;
			left[j].z=P1.z;
			
			right[j].y=yCurr;
			right[j].x=P2.x;
			right[j].z=P2.z;
			
			C.addTriangle(left[j-1], right[j-1], left[j]);
			C.addTriangle(right[j-1], right[j], left[j]);
		}
	}
	
}


///
// makeCone - Create polygons for a cone with unit height, centered at the
// origin, with separate number of radial subdivisions and height 
// subdivisions.
//
// @param radius - Radius of the base of the cone
// @param radialDivision - number of subdivisions on the radial base
// @param heightDivisions - number of subdivisions along the height
//
// Can only use calls to C.addTriangle()
///
void makeCone( Canvas &C, float radius, int radialDivisions, int heightDivisions )
{
    if( radialDivisions < 3 )
        radialDivisions = 3;

    if( heightDivisions < 1 )
        heightDivisions = 1;

    // YOUR IMPLEMENTATION HERE
    float discY[radialDivisions], discX[radialDivisions];
    //disc math
    double alpha=0;
    Vertex front[radialDivisions+1];
    front[radialDivisions].y=0;
    front[radialDivisions].x=0;
    front[radialDivisions].z=0.5;
    for(int i=0;i<radialDivisions;i++){
		discY[i]=radius*sin(alpha);
		discX[i]=radius*cos(alpha);
		front[i].x=discX[i];
		front[i].y=discY[i];
		front[i].z=0.5;

		alpha+=2*PI/((double)radialDivisions);
	}
	//draw disc
	for(int i=0;i<radialDivisions;i++){
		if(i==radialDivisions-1){
			C.addTriangle(front[radialDivisions], front[i], front[0]);
		}else{
			C.addTriangle(front[radialDivisions], front[i], front[i+1]);
		}
	}
	Vertex apex={0, 0, -0.5};
	
	
	Vertex P1, P2;
	for(int i=0;i<radialDivisions;i++){//loop faces
		Vertex left[heightDivisions+1];
		Vertex right[heightDivisions+1];
		if(i==radialDivisions-1){//last face
			P1=front[0], P2=front[i];
		}else{
			P1=front[i+1], P2=front[i];
		}
		left[0]=P1;
		right[0]=P2;
		float zCurr=0.5;
		for(int j=1;j<=heightDivisions;j++){//loop divisions
			if(j==heightDivisions){
				C.addTriangle(apex, left[j-1], right[j-1]);
			}else{
				zCurr-=1.0/((float)heightDivisions);
				alpha=zCurr+0.5;
				
				left[j].z=zCurr;
				left[j].x=((1-alpha)*apex.x)+(alpha*left[0].x);
				left[j].y=((1-alpha)*apex.y)+(alpha*left[0].y);
				
				right[j].z=zCurr;
				right[j].x=((1-alpha)*apex.x)+(alpha*right[0].x);
				right[j].y=((1-alpha)*apex.y)+(alpha*right[0].y);
				
				C.addTriangle(left[j-1], right[j-1], left[j]);
				C.addTriangle(right[j-1], right[j], left[j]);
			}
		}
	}
}


///
// makeSphere - Create sphere of a given radius, centered at the origin, 
// using spherical coordinates with separate number of thetha and 
// phi subdivisions.
//
// @param radius - Radius of the sphere
// @param slices - number of subdivisions in the theta direction
// @param stacks - Number of subdivisions in the phi direction.
//
// Can only use calls to C.addTriangle()
///
void makeSphere( Canvas &C, float radius, int slices, int stacks )
{
    // IF DOING RECURSIVE SUBDIVISION:
    //   use a minimum value of 1 instead of 3
    //   add an 'else' clause to set a maximum value of 5
    if( slices < 3 )
        slices = 3;

    if( stacks < 3 )
        stacks = 3;
    
    // YOUR IMPLEMENTATION HERE
    Vertex top={0, 0.5, 0}, bot={0, -0.5, 0};
    Vertex sphere[stacks+1][slices];
    double phi=0, theta=0;
    for(int i=1;i<stacks;i++){//create vertices
		phi+=PI/((float)stacks);
		for(int j=0;j<slices;j++){
			theta+=2*PI/((float)slices);
			sphere[i][j].x=radius*cos(theta)*sin(phi);
			sphere[i][j].z=radius*sin(theta)*sin(phi);
			sphere[i][j].y=radius*cos(phi);
		}
		
		theta=0;
	}
	//draw this ball
	for(int i=1;i<=stacks;i++){
		if(i==stacks){//bottom stack
			for(int j=0;j<slices;j++){
				if(j==slices-1){
					C.addTriangle(bot, sphere[i-1][j], sphere[i-1][0]);
				}else{
					C.addTriangle(bot, sphere[i-1][j], sphere[i-1][j+1]);
				}
			}
		}else{
			if(i==1){//top stack
				for(int j=0;j<slices;j++){
					if(j==slices-1){
						C.addTriangle(top, sphere[i][0], sphere[i][j]);
					}else{
						C.addTriangle(top, sphere[i][j+1], sphere[i][j]);
					}
				}
			}else{//everything in between
				for(int j=0;j<slices;j++){
					Vertex ll, lr, ul, ur;
					if(j==slices-1){
						ll=sphere[i-1][j], lr=sphere[i-1][0], 
						ul=sphere[i][j], ur=sphere[i][0];
					}else{
						ll=sphere[i-1][j], lr=sphere[i-1][j+1], 
						ul=sphere[i][j], ur=sphere[i][j+1];
					}
					C.addTriangle(ll, lr, ul);
					C.addTriangle(lr, ur, ul);
				}
			}
		
		}
	}
}
