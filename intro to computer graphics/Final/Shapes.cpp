//
// Shapes.cpp
//
// Routines for shape-drawing functions.
//
// This file should not be modified by students.
//

#include <cmath>
#include <iostream>
using namespace std;

#include "Canvas.h"
#include "Shapes.h"
#include "Vertex.h"

#define PI 3.14159265

// makeCube - Create a unit cube, centered at the origin, with a given number
// of subdivisions in each direction on each face.
//
// @param subdivision - number of equal subdivisions to be made in each 
//        direction along each face
//
// Can only use calls to C.addTriangle()
///

void makeCube2( Canvas &C){
	Vertex v1 = { 1.0f, -1.0f, -1.0f};
	Vertex v2 = { 1.0f, -1.0f,  1.0f};
	Vertex v3 = {-1.0f, -1.0f,  1.0f};
	Vertex v4 = {-1.0f, -1.0f, -1.0f};
	Vertex v5 = { 1.0f,  1.0f, -1.0f};
	Vertex v6 = { 1.0f,  1.0f,  1.0f};
	Vertex v7 = {-1.0f,  1.0f,  1.0f};
	Vertex v8 = {-1.0f,  1.0f, -1.0f};
		
	Normal n1 = { 0.0f, -1.0f,  0.0f};
	Normal n2 = { 0.0f,  1.0f,  0.0f};
	Normal n3 = { 1.0f,  0.0f,  0.0f};
	Normal n4 = { 0.0f,  0.0f,  1.0f};
	Normal n5 = {-1.0f,  0.0f,  0.0f};
	Normal n6 = { 0.0f,  0.0f,  -1.0f};
	
		
	C.addTriangleWithNorms(v2, v2, v4, v4, v1, v1);
	C.addTriangleWithNorms(v2, v2, v3, v3, v4, v4);
	
	C.addTriangleWithNorms(v8, v8, v6, v6, v5, v5);
	C.addTriangleWithNorms(v8, v8, v7, v7, v6, v6);
	
	C.addTriangleWithNorms(v5, v5, v2, v2, v1, v1);
	C.addTriangleWithNorms(v5, v5, v6, v6, v2, v2);
	
	C.addTriangleWithNorms(v6, v6, v3, v3, v2, v2);
	C.addTriangleWithNorms(v6, v6, v7, v7, v3, v3);
	
	C.addTriangleWithNorms(v3, v3, v8, v8, v4, v4);
	C.addTriangleWithNorms(v3, v3, v7, v7, v8, v8);
	
	C.addTriangleWithNorms(v1, v1, v8, v8, v5, v5);
	C.addTriangleWithNorms(v1, v1, v4, v4, v8, v8);
}
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
	Normal n1 = {0.0f, 0.0f, 1.0f};
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangleWithNorms(P[i][j], n1, P[i][j+1], n1, P[i+1][j+1], n1);//upper triangle
			C.addTriangleWithNorms(P[i+1][j+1], n1, P[i+1][j], n1, P[i][j], n1);//lower triangle
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
	Normal n2 = {0.0f, 0.0f, -1.0f};
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangleWithNorms(P[i][j], n2, P[i][j+1], n2, P[i+1][j+1], n2);//upper triangle
			C.addTriangleWithNorms(P[i+1][j+1], n2, P[i+1][j], n2, P[i][j], n2);//lower triangle
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
	Normal n3 = {1.0f, 0.0f, 0.0f};
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangleWithNorms(P[i][j], n3, P[i][j+1], n3, P[i+1][j+1], n3);//upper triangle
			C.addTriangleWithNorms(P[i+1][j+1], n3, P[i+1][j], n3, P[i][j], n3);//lower triangle
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
	Normal n4 = {-1.0f, 0.0f, 0.0f};
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangleWithNorms(P[i][j], n4, P[i][j+1], n4, P[i+1][j+1], n4);//upper triangle
			C.addTriangleWithNorms(P[i+1][j+1], n4, P[i+1][j], n4, P[i][j], n4);//lower triangle
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
	Normal n5 = {0.0f, 1.0f, 0.0f};
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangleWithNorms(P[i][j], n5, P[i][j+1], n5, P[i+1][j+1], n5);//upper triangle
			C.addTriangleWithNorms(P[i+1][j+1], n5, P[i+1][j], n5, P[i][j], n5);//lower triangle
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
	Normal n6 = {0.0f, -1.0f, 0.0f};
	for(int i=0;i<subdivisions;i++){//add triangles
		for(int j=0;j<subdivisions;j++){
			C.addTriangleWithNorms(P[i][j], n6, P[i][j+1], n6, P[i+1][j+1], n6);//upper triangle
			C.addTriangleWithNorms(P[i+1][j+1], n6, P[i+1][j], n6, P[i][j], n6);//lower triangle
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
    Normal topN[radialDivisions+1], botN[radialDivisions+1];
    top[radialDivisions].y=0.5;
    top[radialDivisions].x=0;//centerpoints for drawing discs
    top[radialDivisions].z=0;
    topN[radialDivisions]={1.0, 0, 0};
    
    bot[radialDivisions].y=-.5;
    bot[radialDivisions].x=0;
    bot[radialDivisions].z=0;
    botN[radialDivisions]={-1.0, 0, 0};
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
	for(int i=0;i<radialDivisions;i++){//circumference of discs
		topN[i].x=top[i].x;
		topN[i].y=0.5;
		topN[i].z=top[i].z;
		
		botN[i].x=bot[i].x;
		botN[i].y=-0.5;
		botN[i].z=bot[i].z;
	}
	//draw discs
	for(int i=0;i<radialDivisions;i++){
		if(i==radialDivisions-1){
			C.addTriangleWithNorms(top[radialDivisions], topN[radialDivisions], top[i], topN[i], top[0], topN[0]);
			C.addTriangleWithNorms(bot[radialDivisions], botN[radialDivisions], bot[0], botN[0], bot[i], botN[i]);
		}else{
			C.addTriangleWithNorms(top[radialDivisions], topN[radialDivisions], top[i], top[i+1], topN[i], topN[i+1]);
			C.addTriangleWithNorms(bot[radialDivisions], botN[radialDivisions], bot[i+1], bot[i], botN[i+1], botN[i]);
		}
	}
	
	//faces
	Vertex P1, P2;
	Normal N1, N2;
	for(int i=0;i<radialDivisions;i++){//loop faces
		Vertex left[heightDivisions+1];
		Vertex right[heightDivisions+1];
		Normal leftN[heightDivisions+1], rightN[heightDivisions+1];
		if(i==radialDivisions-1){//last face
			P1=bot[i], P2=bot[0];
			N1=botN[i], N2=botN[0];
		}else{
			P1=bot[i], P2=bot[i+1];
			N1=botN[i], N2=botN[i+1];
		}
		left[0]=P1;
		right[0]=P2;
		leftN[0]=N1, rightN[0]=N2;
		float yCurr=-0.5;
		for(int j=1;j<=heightDivisions;j++){//loop divisions
			yCurr+=1.0/((float)heightDivisions);
			left[j].y=yCurr, leftN[j].y=0;
			left[j].x=P1.x, leftN[j].x=N1.x;
			left[j].z=P1.z, leftN[j].z=N1.z;
			
			right[j].y=yCurr, rightN[j].y=0;
			right[j].x=P2.x, rightN[j].x=P2.x;
			right[j].z=P2.z, rightN[j].z=P2.z;
			
			C.addTriangleWithNorms(left[j-1], leftN[j-1], right[j-1], rightN[j-1], left[j], leftN[j]);
			C.addTriangleWithNorms(right[j-1], rightN[j-1], right[j], rightN[j], left[j], leftN[j]);
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
    Normal frontN[radialDivisions+1];
    front[radialDivisions].y=0;
    front[radialDivisions].x=0;
    front[radialDivisions].z=0.5;
    frontN[radialDivisions]={0, 0, 1.0};
    for(int i=0;i<radialDivisions;i++){
		discY[i]=radius*sin(alpha);
		discX[i]=radius*cos(alpha);
		front[i].x=discX[i];
		front[i].y=discY[i];
		front[i].z=0.5;
		frontN[i]={discX[i], discY[i], 0.25};

		alpha+=2*PI/((double)radialDivisions);
	}
	//draw disc
	for(int i=0;i<radialDivisions;i++){
		if(i==radialDivisions-1){
			C.addTriangleWithNorms(front[radialDivisions], frontN[radialDivisions], front[i], frontN[i], front[0], frontN[0]);
		}else{
			C.addTriangleWithNorms(front[radialDivisions], frontN[radialDivisions], front[i], frontN[i], front[i+1], frontN[0]);
		}
	}
	Vertex apex={0, 0, -0.5};
	Normal apexN={0, 0, -1.0};
	
	
	Vertex P1, P2;
	Normal N1, N2;
	for(int i=0;i<radialDivisions;i++){//loop faces
		Vertex left[heightDivisions+1];
		Vertex right[heightDivisions+1];
		Normal leftN[heightDivisions+1], rightN[heightDivisions+1];
		if(i==radialDivisions-1){//last face
			P1=front[0], P2=front[i];
			N1=frontN[0], N2=frontN[i];
		}else{
			P1=front[i+1], P2=front[i];
			N1=frontN[i+1], N2=frontN[i];
		}
		left[0]=P1;
		right[0]=P2;
		leftN[0]=N1;
		rightN[0]=N2;
		float zCurr=0.5;
		for(int j=1;j<=heightDivisions;j++){//loop divisions
			if(j==heightDivisions){
				C.addTriangleWithNorms(apex, apexN, left[j-1], leftN[j-1], right[j-1], rightN[j-1]);
			}else{
				zCurr-=1.0/((float)heightDivisions);
				alpha=zCurr+0.5;
				
				left[j].z=zCurr;
				left[j].x=((1-alpha)*apex.x)+(alpha*left[0].x);
				left[j].y=((1-alpha)*apex.y)+(alpha*left[0].y);
				leftN[j]={left[j].x, left[j].y, -0.5};
				
				right[j].z=zCurr;
				right[j].x=((1-alpha)*apex.x)+(alpha*right[0].x);
				right[j].y=((1-alpha)*apex.y)+(alpha*right[0].y);
				rightN[j]={right[j].x, right[j].y, -0.5};
				
				C.addTriangleWithNorms(left[j-1], leftN[j-1], right[j-1], rightN[j-1], left[j], leftN[j]);
				C.addTriangleWithNorms(right[j-1], rightN[j-1], right[j], rightN[j], left[j], leftN[j]);
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
					C.addTriangleWithNorms(bot, bot, sphere[i-1][j], sphere[i-1][j], sphere[i-1][0], sphere[i-1][0]);
				}else{
					C.addTriangleWithNorms(bot, bot, sphere[i-1][j], sphere[i-1][j], sphere[i-1][j+1], sphere[i-1][j+1]);
				}
			}
		}else{
			if(i==1){//top stack
				for(int j=0;j<slices;j++){
					if(j==slices-1){
						C.addTriangleWithNorms(top, top, sphere[i][0], sphere[i][0], sphere[i][j], sphere[i][j]);
					}else{
						C.addTriangleWithNorms(top, top, sphere[i][j+1], sphere[i][j+1], sphere[i][j], sphere[i][j]);
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
					C.addTriangleWithNorms(ll, ll, lr, lr, ul, ul);
					C.addTriangleWithNorms(lr, lr, ur, ur, ul, ul);
				}
			}
		
		}
	}
}


//
// Each group of three values specifies a teapot vertex
//
float teapotVertices[] = {
     0.071784f,  0.360000f,  0.213696f,  0.067946f,  0.374175f,  0.202270f,
     0.066060f,  0.369450f,  0.196657f,  0.000000f,  0.360000f,  0.210000f,
     0.124723f,  0.360000f,  0.169949f,  0.182088f,  0.360000f,  0.133632f,
     0.172352f,  0.374175f,  0.126487f,  0.167570f,  0.369450f,  0.122977f,
     0.199450f,  0.360000f,  0.066998f,  0.225000f,  0.360000f,  0.000000f,
     0.212970f,  0.374175f,  0.000000f,  0.207060f,  0.369450f,  0.000000f,
    -0.225000f,  0.360000f,  0.000000f, -0.212970f,  0.374175f,  0.000000f,
    -0.207060f,  0.369450f,  0.000000f, -0.199450f,  0.360000f,  0.066998f,
    -0.182088f,  0.360000f,  0.133632f, -0.172352f,  0.374175f,  0.126487f,
    -0.167570f,  0.369450f,  0.122977f, -0.124723f,  0.360000f,  0.169949f,
    -0.071784f,  0.360000f,  0.213696f, -0.067946f,  0.374175f,  0.202270f,
    -0.066060f,  0.369450f,  0.196657f, -0.071784f,  0.360000f, -0.213696f,
    -0.067946f,  0.374175f, -0.202270f, -0.066060f,  0.369450f, -0.196657f,
     0.000000f,  0.360000f, -0.210000f, -0.124723f,  0.360000f, -0.169949f,
    -0.182088f,  0.360000f, -0.133632f, -0.172352f,  0.374175f, -0.126487f,
    -0.167570f,  0.369450f, -0.122977f, -0.199450f,  0.360000f, -0.066998f,
     0.199450f,  0.360000f, -0.066998f,  0.182088f,  0.360000f, -0.133632f,
     0.172352f,  0.374175f, -0.126487f,  0.167570f,  0.369450f, -0.122977f,
     0.124723f,  0.360000f, -0.169949f,  0.071784f,  0.360000f, -0.213696f,
     0.067946f,  0.374175f, -0.202270f,  0.066060f,  0.369450f, -0.196657f,
     0.094372f,  0.176760f,  0.280939f,  0.085375f,  0.266220f,  0.254156f,
     0.239385f,  0.176760f,  0.175682f,  0.216563f,  0.266220f,  0.158933f,
     0.295800f,  0.176760f,  0.000000f,  0.267600f,  0.266220f,  0.000000f,
    -0.284928f,  0.135000f,  0.095712f, -0.295800f,  0.176760f,  0.000000f,
    -0.300000f,  0.135000f,  0.000000f, -0.267600f,  0.266220f,  0.000000f,
    -0.239385f,  0.176760f,  0.175682f, -0.216563f,  0.266220f,  0.158933f,
    -0.094372f,  0.176760f,  0.280939f, -0.085375f,  0.266220f,  0.254156f,
    -0.094372f,  0.176760f, -0.280939f, -0.085375f,  0.266220f, -0.254156f,
    -0.239385f,  0.176760f, -0.175682f, -0.216563f,  0.266220f, -0.158933f,
    -0.284928f,  0.135000f, -0.095712f,  0.239385f,  0.176760f, -0.175682f,
     0.216563f,  0.266220f, -0.158933f,  0.094372f,  0.176760f, -0.280939f,
     0.085375f,  0.266220f, -0.254156f,  0.071784f,  0.022500f,  0.213696f,
     0.000000f,  0.032040f,  0.232800f,  0.000000f,  0.022500f,  0.225000f,
     0.080207f,  0.047520f,  0.238770f,  0.093223f,  0.098460f,  0.277520f,
     0.133632f,  0.022500f,  0.182088f,  0.138265f,  0.032040f,  0.188400f,
     0.182088f,  0.022500f,  0.133632f,  0.203453f,  0.047520f,  0.149311f,
     0.236472f,  0.098460f,  0.173543f,  0.225000f,  0.022500f,  0.000000f,
     0.251400f,  0.047520f,  0.000000f,  0.292200f,  0.098460f,  0.000000f,
    -0.225000f,  0.022500f,  0.000000f, -0.251400f,  0.047520f,  0.000000f,
    -0.292200f,  0.098460f,  0.000000f, -0.182088f,  0.022500f,  0.133632f,
    -0.203453f,  0.047520f,  0.149311f, -0.236472f,  0.098460f,  0.173543f,
    -0.133632f,  0.022500f,  0.182088f, -0.138265f,  0.032040f,  0.188400f,
    -0.071784f,  0.022500f,  0.213696f, -0.080207f,  0.047520f,  0.238770f,
    -0.093223f,  0.098460f,  0.277520f, -0.071784f,  0.022500f, -0.213696f,
     0.000000f,  0.032040f, -0.232800f,  0.000000f,  0.022500f, -0.225000f,
    -0.080207f,  0.047520f, -0.238770f, -0.093223f,  0.098460f, -0.277520f,
    -0.133632f,  0.022500f, -0.182088f, -0.138265f,  0.032040f, -0.188400f,
    -0.182088f,  0.022500f, -0.133632f, -0.203453f,  0.047520f, -0.149311f,
    -0.236472f,  0.098460f, -0.173543f,  0.182088f,  0.022500f, -0.133632f,
     0.203453f,  0.047520f, -0.149311f,  0.236472f,  0.098460f, -0.173543f,
     0.133632f,  0.022500f, -0.182088f,  0.138265f,  0.032040f, -0.188400f,
     0.071784f,  0.022500f, -0.213696f,  0.080207f,  0.047520f, -0.238770f,
     0.093223f,  0.098460f, -0.277520f, -0.445320f,  0.270000f,  0.021600f,
    -0.410169f,  0.320168f,  0.021600f, -0.305114f,  0.333478f,  0.021600f,
    -0.225000f,  0.337500f,  0.000000f, -0.230280f,  0.325620f,  0.032400f,
    -0.420840f,  0.270000f,  0.032400f, -0.390128f,  0.305774f,  0.032400f,
    -0.300479f,  0.315265f,  0.032400f, -0.238440f,  0.307260f,  0.021600f,
    -0.405000f,  0.270000f,  0.000000f, -0.377160f,  0.296460f,  0.000000f,
    -0.297480f,  0.303480f,  0.000000f, -0.238440f,  0.307260f, -0.021600f,
    -0.420840f,  0.270000f, -0.032400f, -0.390128f,  0.305774f, -0.032400f,
    -0.300479f,  0.315265f, -0.032400f, -0.230280f,  0.325620f, -0.032400f,
    -0.445320f,  0.270000f, -0.021600f, -0.410169f,  0.320168f, -0.021600f,
    -0.305114f,  0.333478f, -0.021600f, -0.344773f,  0.125536f,  0.021600f,
    -0.285000f,  0.090000f,  0.000000f, -0.420713f,  0.194718f,  0.021600f,
    -0.290280f,  0.105840f,  0.032400f, -0.341705f,  0.145708f,  0.032400f,
    -0.401847f,  0.208623f,  0.032400f, -0.298440f,  0.130320f,  0.021600f,
    -0.339720f,  0.158760f,  0.000000f, -0.389640f,  0.217620f,  0.000000f,
    -0.298440f,  0.130320f, -0.021600f, -0.341705f,  0.145708f, -0.032400f,
    -0.401847f,  0.208623f, -0.032400f, -0.290280f,  0.105840f, -0.032400f,
    -0.344773f,  0.125536f, -0.021600f, -0.420713f,  0.194718f, -0.021600f,
     0.485640f,  0.360000f,  0.018000f,  0.407738f,  0.254405f,  0.028391f,
     0.344420f,  0.135936f,  0.044450f,  0.255000f,  0.090000f,  0.000000f,
     0.255000f,  0.133560f,  0.071280f,  0.436680f,  0.360000f,  0.027000f,
     0.381887f,  0.275458f,  0.042587f,  0.327578f,  0.189792f,  0.066675f,
     0.255000f,  0.200880f,  0.047520f,  0.405000f,  0.360000f,  0.000000f,
     0.365160f,  0.289080f,  0.000000f,  0.316680f,  0.224640f,  0.000000f,
     0.255000f,  0.200880f, -0.047520f,  0.436680f,  0.360000f, -0.027000f,
     0.381887f,  0.275458f, -0.042587f,  0.327578f,  0.189792f, -0.066675f,
     0.255000f,  0.133560f, -0.071280f,  0.485640f,  0.360000f, -0.018000f,
     0.407738f,  0.254405f, -0.028391f,  0.344420f,  0.135936f, -0.044450f,
     0.491550f,  0.367577f,  0.011549f,  0.480000f,  0.360000f,  0.000000f,
     0.505322f,  0.370640f,  0.015466f,  0.458880f,  0.360000f,  0.016200f,
     0.451533f,  0.366255f,  0.017323f,  0.454175f,  0.369098f,  0.023198f,
     0.426240f,  0.360000f,  0.010800f,  0.425640f,  0.365400f,  0.000000f,
     0.421080f,  0.368100f,  0.000000f,  0.426240f,  0.360000f, -0.010800f,
     0.451533f,  0.366255f, -0.017323f,  0.454175f,  0.369098f, -0.023198f,
     0.458880f,  0.360000f, -0.016200f,  0.491550f,  0.367577f, -0.011549f,
     0.505322f,  0.370640f, -0.015466f,  0.009571f,  0.405000f,  0.028493f,
     0.013127f,  0.438480f,  0.038986f,  0.014822f,  0.467640f,  0.044004f,
     0.000000f,  0.472500f,  0.000000f,  0.024278f,  0.405000f,  0.017818f,
     0.033238f,  0.438480f,  0.024412f,  0.037519f,  0.467640f,  0.027560f,
     0.030000f,  0.405000f,  0.000000f,  0.041040f,  0.438480f,  0.000000f,
     0.046320f,  0.467640f,  0.000000f, -0.030000f,  0.405000f,  0.000000f,
    -0.041040f,  0.438480f,  0.000000f, -0.046320f,  0.467640f,  0.000000f,
    -0.024278f,  0.405000f,  0.017818f, -0.033238f,  0.438480f,  0.024412f,
    -0.037519f,  0.467640f,  0.027560f, -0.009571f,  0.405000f,  0.028493f,
    -0.013127f,  0.438480f,  0.038986f, -0.014822f,  0.467640f,  0.044004f,
    -0.009571f,  0.405000f, -0.028493f, -0.013127f,  0.438480f, -0.038986f,
    -0.014822f,  0.467640f, -0.044004f, -0.024278f,  0.405000f, -0.017818f,
    -0.033238f,  0.438480f, -0.024412f, -0.037519f,  0.467640f, -0.027560f,
     0.024278f,  0.405000f, -0.017818f,  0.033238f,  0.438480f, -0.024412f,
     0.037519f,  0.467640f, -0.027560f,  0.009571f,  0.405000f, -0.028493f,
     0.013127f,  0.438480f, -0.038986f,  0.014822f,  0.467640f, -0.044004f,
     0.057657f,  0.371160f,  0.171641f,  0.000000f,  0.360000f,  0.195000f,
     0.032236f,  0.385920f,  0.095964f,  0.115814f,  0.360000f,  0.157810f,
     0.146253f,  0.371160f,  0.107333f,  0.081770f,  0.385920f,  0.060010f,
     0.185203f,  0.360000f,  0.062213f,  0.180720f,  0.371160f,  0.000000f,
     0.101040f,  0.385920f,  0.000000f, -0.185203f,  0.360000f,  0.062213f,
    -0.180720f,  0.371160f,  0.000000f, -0.101040f,  0.385920f,  0.000000f,
    -0.146253f,  0.371160f,  0.107333f, -0.081770f,  0.385920f,  0.060010f,
    -0.115814f,  0.360000f,  0.157810f, -0.057657f,  0.371160f,  0.171641f,
    -0.032236f,  0.385920f,  0.095964f, -0.057657f,  0.371160f, -0.171641f,
     0.000000f,  0.360000f, -0.195000f, -0.032236f,  0.385920f, -0.095964f,
    -0.115814f,  0.360000f, -0.157810f, -0.146253f,  0.371160f, -0.107333f,
    -0.081770f,  0.385920f, -0.060010f, -0.185203f,  0.360000f, -0.062213f,
     0.185203f,  0.360000f, -0.062213f,  0.146253f,  0.371160f, -0.107333f,
     0.081770f,  0.385920f, -0.060010f,  0.115814f,  0.360000f, -0.157810f,
     0.057657f,  0.371160f, -0.171641f,  0.032236f,  0.385920f, -0.095964f,
     0.071784f,  0.022500f, -0.213696f,  0.000000f,  0.015840f, -0.222120f,
     0.066156f,  0.009720f, -0.196942f,  0.033652f,  0.001260f, -0.100181f,
     0.000000f,  0.000000f,  0.000000f,  0.131922f,  0.015840f, -0.179757f,
     0.167812f,  0.009720f, -0.123155f,  0.085363f,  0.001260f, -0.062647f,
     0.207360f,  0.009720f,  0.000000f,  0.105480f,  0.001260f,  0.000000f,
    -0.207360f,  0.009720f,  0.000000f, -0.105480f,  0.001260f,  0.000000f,
    -0.167812f,  0.009720f, -0.123155f, -0.085363f,  0.001260f, -0.062647f,
    -0.131922f,  0.015840f, -0.179757f, -0.071784f,  0.022500f, -0.213696f,
    -0.066156f,  0.009720f, -0.196942f, -0.033652f,  0.001260f, -0.100181f,
    -0.071784f,  0.022500f,  0.213696f,  0.000000f,  0.015840f,  0.222120f,
    -0.066156f,  0.009720f,  0.196942f, -0.033652f,  0.001260f,  0.100181f,
    -0.131922f,  0.015840f,  0.179757f, -0.167812f,  0.009720f,  0.123155f,
    -0.085363f,  0.001260f,  0.062647f,  0.167812f,  0.009720f,  0.123155f,
     0.085363f,  0.001260f,  0.062647f,  0.131922f,  0.015840f,  0.179757f,
     0.071784f,  0.022500f,  0.213696f,  0.066156f,  0.009720f,  0.196942f,
     0.033652f,  0.001260f,  0.100181f
};

int teapotVerticesLength = sizeof(teapotVertices) / sizeof(float);

//
// Each group of three values specifies a teapot vertex normal
//
float teapotNormals[] = {
    -0.262900f,  0.524100f,  0.810000f,  0.262900f,  0.524100f,  0.810000f,
     0.069100f,  0.973600f,  0.217400f, -0.757100f,  0.355000f, -0.548400f,
    -0.689400f,  0.524700f, -0.499300f, -0.262900f,  0.524100f, -0.810000f,
     0.689400f,  0.524700f,  0.499300f,  0.852000f,  0.523600f,  0.000000f,
     0.227000f,  0.973900f,  0.000000f, -0.409100f, -0.717800f, -0.563400f,
    -0.000000f, -0.734000f, -0.679200f, -0.230300f,  0.660700f, -0.714400f,
    -0.185900f,  0.973400f,  0.133500f,  0.615100f,  0.650500f, -0.445500f,
     0.748000f,  0.663600f,  0.000000f, -0.615100f,  0.650500f, -0.445500f,
    -0.773300f,  0.159600f, -0.613500f, -0.969800f,  0.243700f,  0.000000f,
    -0.935300f,  0.353900f,  0.000000f,  0.803400f,  0.125500f, -0.582100f,
     0.757100f,  0.355000f, -0.548400f,  0.935300f,  0.353900f,  0.000000f,
    -0.650400f, -0.730000f, -0.210000f, -0.748000f,  0.663600f,  0.000000f,
     0.185900f,  0.973400f, -0.133500f, -0.185900f,  0.973400f, -0.133500f,
    -0.227000f,  0.973900f,  0.000000f,  0.689400f,  0.524700f, -0.499300f,
    -0.230300f,  0.660700f,  0.714400f, -0.409100f, -0.717800f,  0.563400f,
    -0.615100f,  0.650500f,  0.445500f, -0.851300f,  0.524600f,  0.000000f,
    -0.946700f, -0.061300f,  0.316000f, -0.211100f, -0.727400f, -0.652900f,
    -0.481700f, -0.552400f, -0.680300f, -0.615000f, -0.661900f, -0.428400f,
     0.016600f, -0.998500f,  0.051700f,  0.044000f, -0.998500f,  0.031800f,
     0.325600f, -0.927800f,  0.181900f, -0.689400f,  0.524700f,  0.499300f,
    -0.080500f,  0.869000f, -0.488200f, -0.464400f,  0.741400f, -0.484300f,
    -0.464400f,  0.741400f,  0.484300f, -0.757100f,  0.355000f,  0.548400f,
    -0.288700f,  0.354400f,  0.889400f,  0.650400f, -0.730000f, -0.210000f,
     0.230300f,  0.660700f,  0.714400f, -0.069100f,  0.973600f, -0.217400f,
    -0.650400f, -0.730000f,  0.210000f,  0.409100f, -0.717800f, -0.563400f,
     0.230300f,  0.660700f, -0.714400f, -0.288700f,  0.354400f, -0.889400f,
     0.262900f,  0.524100f, -0.810000f,  0.757100f,  0.355000f,  0.548400f,
    -0.069100f,  0.973600f,  0.217400f,  0.650400f, -0.730000f,  0.210000f,
     0.615100f,  0.650500f,  0.445500f,  0.409100f, -0.717800f,  0.563400f,
     0.000000f, -0.734000f,  0.679200f,  0.499700f, -0.527500f,  0.687000f,
     0.481700f, -0.552400f,  0.680300f,  0.294500f,  0.275600f,  0.915100f,
    -0.723800f, -0.366900f,  0.584400f, -0.615000f, -0.661900f,  0.428400f,
    -0.211100f, -0.727400f,  0.652900f, -0.946700f, -0.061300f, -0.316000f,
    -0.852000f,  0.523600f,  0.000000f,  0.069100f,  0.973600f, -0.217400f,
     0.803400f,  0.125500f,  0.582100f,  0.763000f, -0.334900f,  0.552800f,
     0.942400f, -0.334400f,  0.000000f,  0.561500f, -0.756500f,  0.335200f,
     0.615000f, -0.661900f,  0.428400f, -0.741900f, -0.670400f,  0.000000f,
     0.291300f, -0.334600f,  0.896200f,  0.306600f,  0.125300f,  0.943500f,
    -0.306600f,  0.125300f,  0.943500f,  0.992100f,  0.125200f,  0.000000f,
    -0.080500f,  0.869000f,  0.488200f,  0.031100f,  0.999500f, -0.000000f,
     0.763000f, -0.334900f, -0.552800f, -0.646200f, -0.763100f,  0.000000f,
    -0.773300f,  0.159600f,  0.613500f,  0.288700f,  0.354400f,  0.889400f,
     0.481700f, -0.552400f, -0.680300f,  0.211100f, -0.727400f, -0.652900f,
     0.615000f, -0.661900f, -0.428400f, -0.291300f, -0.334600f, -0.896200f,
    -0.306600f,  0.125300f, -0.943500f,  0.306600f,  0.125300f, -0.943500f,
     0.000000f, -0.633000f,  0.774200f,  0.000000f, -0.536100f,  0.844100f,
    -0.891300f, -0.453500f,  0.000000f,  0.000000f, -0.536100f, -0.844100f,
    -0.723800f, -0.366900f, -0.584400f,  0.741900f, -0.670400f,  0.000000f,
     0.288700f,  0.354400f, -0.889400f,  0.291300f, -0.334600f, -0.896200f,
     0.646200f, -0.763100f,  0.000000f,  0.211100f, -0.727400f,  0.652900f,
     0.126200f, -0.103500f,  0.986600f, -0.863700f,  0.127200f,  0.487700f,
    -0.000000f, -0.633000f, -0.774200f, -0.294500f,  0.275600f, -0.915100f,
     0.561500f, -0.756500f, -0.335200f, -0.561500f, -0.756500f,  0.335200f,
     0.499700f, -0.527500f, -0.687000f,  0.294500f,  0.275600f, -0.915100f,
    -0.499700f, -0.527500f,  0.687000f, -0.481700f, -0.552400f,  0.680300f,
    -0.294500f,  0.275600f,  0.915100f, -0.249500f, -0.630500f, -0.735000f,
    -0.530200f, -0.706200f, -0.469100f,  0.104600f,  0.226800f, -0.968300f,
     0.953900f, -0.300100f,  0.000000f,  0.109900f,  0.010700f,  0.993900f,
     0.209200f,  0.117900f,  0.970700f,  0.209200f,  0.117900f, -0.970700f,
     0.109900f,  0.010700f, -0.993900f, -0.863700f,  0.127200f, -0.487700f,
    -0.725300f, -0.469600f, -0.503300f, -0.725300f, -0.469600f,  0.503300f,
    -0.499700f, -0.527500f, -0.687000f, -0.561500f, -0.756500f, -0.335200f,
    -0.291300f, -0.334600f,  0.896200f, -0.325600f, -0.927800f, -0.181900f,
    -0.392400f, -0.919800f,  0.000000f,  0.437000f,  0.685400f,  0.582500f,
     0.637500f,  0.770400f,  0.000000f,  0.104600f,  0.226800f,  0.968300f,
    -0.025500f, -0.320200f,  0.947000f, -0.219100f,  0.494600f,  0.841000f,
    -0.749000f,  0.662500f,  0.000000f, -0.797300f,  0.603500f,  0.000000f,
     0.388900f, -0.921300f,  0.000000f,  0.057200f, -0.848100f, -0.526700f,
     0.028200f, -0.999600f,  0.000000f, -0.025500f, -0.320200f, -0.947000f,
    -0.249500f, -0.630500f,  0.735000f,  0.069000f,  0.512600f,  0.855900f,
     0.860100f,  0.292900f, -0.417600f,  0.032200f,  0.858400f, -0.511900f,
     0.174100f,  0.593200f,  0.786000f,  0.057200f, -0.848100f,  0.526700f,
     0.126200f, -0.103500f, -0.986600f, -0.433400f, -0.901200f, -0.000000f,
    -0.530200f, -0.706200f,  0.469100f,  0.069000f,  0.512600f, -0.855900f,
     0.437000f,  0.685400f, -0.582500f,  0.142800f,  0.249000f,  0.957900f,
    -0.215400f,  0.300500f,  0.929100f,  0.513400f,  0.858200f,  0.000000f,
     0.860100f,  0.292900f,  0.417600f,  0.532500f, -0.496200f,  0.685700f,
     0.532500f, -0.496200f, -0.685700f,  0.873200f,  0.487400f,  0.000000f,
     0.032200f,  0.858400f,  0.511900f,  0.291400f, -0.956600f,  0.000000f,
     0.603700f, -0.622900f, -0.497400f,  0.603700f, -0.622900f,  0.497400f,
     0.735400f, -0.394800f, -0.550700f,  0.735400f, -0.394800f,  0.550700f,
     0.101000f, -0.421300f, -0.901300f,  0.101000f, -0.421300f,  0.901300f,
    -0.219100f,  0.494600f, -0.841000f,  0.785400f, -0.244400f,  0.568600f,
     0.678500f,  0.546700f,  0.490600f,  0.838100f,  0.545500f,  0.000000f,
    -0.107800f,  0.246100f,  0.963200f, -0.223300f,  0.940600f,  0.255700f,
     0.785400f, -0.244400f, -0.568600f,  0.970000f, -0.243100f,  0.000000f,
    -0.205700f,  0.774800f, -0.597800f, -0.531800f,  0.846800f,  0.000000f,
     0.142800f,  0.249000f, -0.957900f, -0.205700f,  0.774800f,  0.597800f,
    -0.299200f, -0.243800f,  0.922500f,  0.299200f, -0.243800f,  0.922500f,
     0.260100f,  0.539200f,  0.801000f, -0.361000f,  0.932600f, -0.000000f,
    -0.223300f,  0.940600f, -0.255700f, -0.215400f,  0.300500f, -0.929100f,
     0.936600f,  0.350500f,  0.000000f,  0.540100f, -0.672500f,  0.506000f,
     0.174100f,  0.593200f, -0.786000f, -0.107800f,  0.246100f, -0.963200f,
     0.841600f,  0.540000f,  0.000000f,  0.682500f,  0.538400f,  0.494200f,
    -0.243400f,  0.969900f,  0.000000f,  0.540100f, -0.672500f, -0.506000f,
     0.000000f,  1.000000f,  0.000000f,  0.260100f,  0.539200f, -0.801000f,
    -0.260100f,  0.539200f, -0.801000f,  0.257500f,  0.546300f,  0.797000f,
    -0.260100f,  0.539200f,  0.801000f, -0.065600f,  0.977000f, -0.202600f,
    -0.257500f,  0.546300f, -0.797000f,  0.257500f,  0.546300f, -0.797000f,
    -0.257500f,  0.546300f,  0.797000f, -0.065600f,  0.977000f,  0.202600f,
     0.065600f,  0.977000f,  0.202600f, -0.306400f,  0.853300f, -0.421900f,
    -0.189000f,  0.788600f, -0.585200f, -0.000000f,  0.858000f, -0.513600f,
    -0.970000f, -0.243100f,  0.000000f, -0.785400f, -0.244400f,  0.568600f,
    -0.682500f,  0.538400f,  0.494200f,  0.172200f,  0.977100f,  0.124600f,
     0.213400f,  0.976900f,  0.000000f,  0.189000f,  0.788600f, -0.585200f,
    -0.785400f, -0.244400f, -0.568600f, -0.678500f,  0.546700f, -0.490600f,
    -0.838100f,  0.545500f,  0.000000f, -0.172200f,  0.977100f,  0.124600f,
    -0.678500f,  0.546700f,  0.490600f,  0.189000f,  0.788600f,  0.585200f,
     0.487600f,  0.798000f,  0.354000f,  0.299200f, -0.243800f, -0.922500f,
     0.682500f,  0.538400f, -0.494200f,  0.619100f,  0.785300f,  0.000000f,
    -0.682500f,  0.538400f, -0.494200f, -0.299200f, -0.243800f, -0.922500f,
     0.490400f,  0.856900f,  0.158900f, -0.487600f,  0.798000f,  0.354000f,
     0.172200f,  0.977100f, -0.124600f,  0.065600f,  0.977000f, -0.202600f,
    -0.306400f,  0.853300f,  0.421900f, -0.189000f,  0.788600f,  0.585200f,
    -0.172200f,  0.977100f, -0.124600f,  0.325600f, -0.927800f, -0.181900f,
     0.392400f, -0.919800f,  0.000000f,  0.054400f, -0.998500f,  0.000000f,
     0.490400f,  0.856900f, -0.158900f,  0.487600f,  0.798000f, -0.354000f,
    -0.841600f,  0.540000f,  0.000000f,  0.000000f, -1.000000f,  0.000000f,
    -0.016600f, -0.998500f,  0.051700f,  0.000000f, -0.396900f, -0.917900f,
     0.145000f, -0.881100f, -0.450100f,  0.000000f, -0.736900f, -0.675900f,
     0.000000f,  0.858000f,  0.513600f,  0.678500f,  0.546700f, -0.490600f,
     0.306400f,  0.853300f,  0.421900f, -0.396400f, -0.735800f,  0.549000f,
    -0.145000f, -0.881100f,  0.450100f, -0.619100f,  0.785300f,  0.000000f,
    -0.490400f,  0.856900f, -0.158900f, -0.490400f,  0.856900f,  0.158900f,
    -0.487600f,  0.798000f, -0.354000f, -0.213400f,  0.976900f,  0.000000f,
    -0.158500f, -0.856000f, -0.492100f,  0.158500f, -0.856000f, -0.492100f,
     0.306400f,  0.853300f, -0.421900f, -0.016600f, -0.998500f, -0.051700f,
     0.016600f, -0.998500f, -0.051700f,  0.044000f, -0.998500f, -0.031800f,
    -0.044000f, -0.998500f, -0.031800f, -0.054400f, -0.998500f,  0.000000f,
     0.396400f, -0.735800f, -0.549000f,  0.158500f, -0.856000f,  0.492100f,
    -0.158500f, -0.856000f,  0.492100f, -0.325600f, -0.927800f,  0.181900f,
    -0.145000f, -0.881100f, -0.450100f, -0.396400f, -0.735800f, -0.549000f,
     0.000000f, -0.396900f,  0.917900f,  0.000000f, -0.736900f,  0.675900f,
    -0.044000f, -0.998500f,  0.031800f,  0.396400f, -0.735800f,  0.549000f,
     0.145000f, -0.881100f,  0.450100f,  0.185900f,  0.973400f,  0.133500f
};

int teapotNormalsLength = sizeof(teapotNormals) / sizeof(float);

//
// Each group of three entries specifies a triangle for the teapot
//
int teapotElements[] = {
     20,   0,   1,  57,  28,  23,   5,   9,  10,   4,   3,   2,  17,  18,  14,
      7,   4,   2,  56,  47,  49,  59,  60,  45,   8,   4,   7,  11,  10,  34,
     29,  13,  14,  10,   9,  33,  39,  36,  35,  48,  46,  47,  90,  93,  95,
    266, 262, 261,  16,  17,  13, 124, 123, 106,  51,  53,  20,  18,  15,  14,
     25,  24,  29,  32,  11,  35,  19,  18,  22,  19,  15,  18,  55,  23,  37,
      3,  19,  22,  43,   5,   0,   8,   7,  11,  21,  17,  16,  37,  23,  24,
     31,  30,  14,  27,  26,  25,  68,  69,  63,  81,  80,  85,  48,  47,  58,
     12,  28,  57,  60,  33,   9,  31,  27,  30,  31,  14,  15,  38,  24,  25,
     32,   8,  11,  13,  29,  28,  42,  72,  75,  39,  35,  34,  36,  32,  35,
     69,  70,  71,  26,  36,  39,  77,  80,  81,  38,  34,  33,  67,  40,  52,
     68,  70,  69,  24,  23,  28,  42,  44,  45, 107, 108, 124,  75,  99,  59,
     76,  77,  95,  53,  51,  50,  40,  41,  53,  37,  33,  60,  50,  51,  49,
    101, 103,  98,  22,  18,  17,  42,  43,  41,  42,  40,  67,  91,  54,  61,
     65,  63,  64,  78,  48,  58,  88,  90, 103,  91,  96,  56,  72,  71,  74,
     50,  46,  81,  99,  98, 103,  62,  60,  59,  59,  99, 104,  71,  70,  73,
     85,  64,  66, 111, 106, 105,  27,  25,  30,  66,  69,  71,  69,  66,  64,
     85,  66,  67,  89,  87,  88,  66,  71,  72,  99,  75,  74,  46,  50,  47,
     73,  97,  98,  76,  79,  80, 100, 102, 101,  82,  83,  79, 101,  98,  97,
     83,  85,  80, 100, 101,  97,  81,  46,  78,  58,  47,  56,  62,  61,  54,
     83,  84,  64,  65,  64,  84,  82,  84,  83, 137, 138, 135, 114, 110, 130,
    136, 118, 114,  96,  58,  56,  88,  87,  93, 122, 139, 127,  92,  94,  93,
     46,  48,  78,  77,  78,  96,  83,  80,  79,  86,  52,  50,  94, 248, 246,
    131, 132, 129,  92,  93,  87, 112, 107, 106,  89,  88, 102,  88, 103, 101,
     56,  57,  55,  91, 104, 103,  58,  96,  78,  95,  93,  94, 105, 106, 123,
    145, 149, 150, 115, 111, 110, 110, 105, 127, 117, 116, 120, 136, 135, 138,
    131, 129, 128, 109, 108, 107, 174, 171, 170, 139, 122, 118, 113, 109, 112,
    119, 118, 122, 119, 120, 116, 117, 113, 116, 138, 126, 125, 125, 129, 130,
    120, 119, 123, 125, 128, 129, 121, 117, 120, 108, 121, 124, 132, 134, 135,
    147, 146, 150, 124, 121, 120, 113, 112, 116, 116, 112, 111, 118, 119, 115,
     48, 134, 132, 125, 127, 139, 162, 140, 157, 135, 136, 133, 140, 145, 146,
     48, 132, 131, 162, 165, 145, 137, 126, 138, 143, 159, 142, 158, 141, 142,
    128, 125, 126, 134, 137, 135, 109, 107, 112, 133, 130, 129, 157, 140, 141,
    143, 156, 159, 144, 143, 142, 150, 149, 153, 180, 179, 182, 172, 173, 170,
    201, 183, 182, 152, 151, 155, 148, 144, 147, 192, 176, 177, 174, 157, 153,
    161, 160, 173, 157, 158, 154, 156, 155, 159, 167, 169, 170, 162, 160, 164,
    156, 152, 155, 161, 163, 160, 183, 184, 181, 147, 144, 142, 152, 148, 151,
    151, 148, 147, 167, 168, 165, 142, 141, 146, 171, 168, 167, 164, 166, 167,
    154, 158, 159, 163, 166, 164, 178, 205, 196, 171, 153, 149, 180, 176, 175,
    150, 154, 155, 166, 169, 167, 172, 161, 173, 178, 193, 177, 169, 172, 170,
    225, 194, 203, 163, 164, 160, 173, 160, 162, 191, 222, 208, 149, 145, 165,
    175, 176, 192, 226, 223, 224, 186, 189, 190, 178, 177, 181, 175, 208, 211,
    211, 214, 182, 234, 223, 225, 198, 197, 185, 219, 188, 185, 208, 206, 210,
    204, 205, 202, 194, 197, 198, 190, 189, 192, 185, 188, 189, 191, 188, 219,
    184, 178, 181, 210, 213, 214, 199, 196, 195, 210, 212, 213, 202, 184, 183,
    181, 177, 176, 222, 219, 218, 189, 188, 191, 232, 235, 203, 194, 195, 204,
    218, 220, 221, 178, 196, 199, 228, 197, 194, 242, 244, 245, 182, 214, 232,
    230, 231, 213, 202, 178, 184, 178, 187, 190, 196, 205, 204, 240, 266, 257,
     89, 236, 237, 221, 207, 206, 201, 200, 203, 221, 206, 208, 214, 213, 231,
    209, 212, 210,  82, 258, 254, 207, 221, 220, 186, 187, 199, 216, 229, 215,
    230, 213, 212, 226, 227, 223, 217, 216, 218, 234, 224, 223, 252, 237, 238,
    178, 199, 187, 216, 215, 218, 193, 178, 190, 231, 233, 234, 240, 253, 239,
    242, 243, 239, 185, 197, 228, 249, 240, 247, 229, 216, 227, 226, 229, 227,
    233, 224, 234, 220, 218, 215, 225, 223, 227, 217, 228, 227, 209, 210, 206,
    233, 231, 230, 100, 241, 236, 178, 202, 205, 235, 232, 231, 207, 209, 206,
    237, 236, 241, 253, 252, 238, 266, 265, 256, 244,  73,  70, 242, 241,  97,
     97,  73, 244, 245, 240, 243, 245, 244, 261,   1,   0,   5, 259, 246, 247,
    257, 256, 259, 248, 252, 253,  89, 237, 251,  92, 250,  94, 248, 250, 252,
    262, 240, 245,  92, 251, 250, 250, 251, 237, 243, 240, 239, 259, 258,  79,
     65, 254, 255, 238, 241, 242,  94, 250, 248, 100,  97, 241, 258, 256, 255,
     20,  53,  41, 256, 258, 259,  26,  39,  25, 240, 260, 247,  82,  79, 258,
    266, 240, 262, 240, 249, 253, 265, 255, 256, 246, 259,  79, 248, 249, 247,
    263, 261,  70, 240, 257, 260,  68, 263,  70,  65, 255, 264,  68, 264, 263,
    255, 265, 263,   3,  22,   2,   1,   2,  22, 263, 265, 261,  96,  91,  90,
      2,   1,   6,  11,   7,   6,   9,   5,  43,  12,  49,  51,  21,  20,   1,
     55,  57,  23,   6,   5,  10,  13,  17,  14,  57,  56,  49,  44,  59,  45,
     35,  11,  34,  30,  29,  14,  34,  10,  33, 265, 266, 261,  12,  16,  13,
    107, 124, 106,  16,  51,  20,  30,  25,  29,  62,  55,  37,  41,  43,   0,
     20,  21,  16,  38,  37,  24,  86,  81,  85,  49,  12,  57,  45,  60,   9,
     39,  38,  25,  12,  13,  28,  44,  42,  75,  38,  39,  34,  78,  77,  81,
     37,  38,  33,  86,  67,  52,  29,  24,  28,  43,  42,  45,  44,  75,  59,
     94,  76,  95,  52,  53,  50,  52,  40,  53,  62,  37,  60,  47,  50,  49,
     21,  22,  17,  40,  42,  41,  72,  42,  67, 104,  91,  61,  54,  91,  56,
     75,  72,  74, 104,  99, 103,  61,  62,  59,  61,  59, 104,  74,  71,  73,
    110, 111, 105,  63,  69,  64,  86,  85,  67,  67,  66,  72,  98,  99,  74,
     74,  73,  98,  77,  76,  80,  55,  62,  54,  85,  83,  64, 133, 114, 130,
    133, 136, 114,  90,  88,  93, 105, 122, 127,  95,  77,  96,  81,  86,  50,
     76,  94, 246, 111, 112, 106, 102,  88, 101,  54,  56,  55,  90,  91, 103,
    122, 105, 123, 146, 145, 150, 114, 115, 110, 130, 110, 127, 139, 136, 138,
    173, 174, 170, 136, 139, 118, 123, 119, 122, 115, 119, 116, 127, 125, 130,
    124, 120, 123, 151, 147, 150, 115, 116, 111, 114, 118, 115, 138, 125, 139,
    174, 162, 157, 132, 135, 133, 141, 140, 146, 140, 162, 145, 159, 158, 142,
    132, 133, 129, 158, 157, 141, 154, 150, 153, 183, 180, 182, 200, 201, 182,
    193, 192, 177, 171, 174, 153, 153, 157, 154, 165, 162, 164, 180, 183, 181,
    164, 167, 165, 147, 142, 146, 170, 171, 167, 155, 154, 159, 168, 171, 149,
    179, 180, 175, 151, 150, 155, 235, 225, 203, 174, 173, 162, 175, 191, 208,
    168, 149, 165, 191, 175, 192, 187, 186, 190, 179, 175, 211, 179, 211, 182,
    235, 234, 225, 186, 198, 185, 217, 219, 185, 211, 208, 210, 201, 204, 202,
    195, 194, 198, 193, 190, 192, 186, 185, 189, 222, 191, 219, 211, 210, 214,
    198, 199, 195, 201, 202, 183, 180, 181, 176, 221, 222, 218, 192, 189, 191,
    200, 232, 203, 203, 194, 204, 225, 228, 194, 243, 242, 245, 200, 182, 232,
    195, 196, 204, 204, 201, 203, 222, 221, 208, 232, 214, 231, 198, 186, 199,
    219, 217, 218, 238, 242, 239, 217, 185, 228, 228, 225, 227, 216, 217, 227,
    234, 235, 231, 238, 237, 241, 239, 253, 238, 257, 266, 256, 261, 244,  70,
    242,  97, 244, 262, 245, 261,   6,   1,   5, 260, 259, 247, 260, 257, 259,
    249, 248, 253, 252, 250, 237, 254, 258, 255,   0,  20,  41,  76, 246,  79,
    246, 248, 247, 264, 255, 263,  21,   1,  22,  95,  96,  90,   7,   2,   6,
     10,  11,   6,  45,   9,  43,  16,  12,  51
};

int teapotElementsLength = sizeof(teapotElements) / sizeof(int);

//
// Each group of three entries specifies the normals used to create
// a single averaged teapot vertex normal
//
int teapotNormalIndices[] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,   9,  11,  16,  17,  18,  19,  20,  21,  22,   9,  15,  23,   8,  24,
     25,  26,  14,   8,   7,  27,  28,  29,  30,  31,  32,  17,  33,  34,  35,
     36,  37,  38,  39,  12,  26,  40,  41,  42,  43,  44,   0,  13,  45,  14,
     46,  47,  25,  48,  23,  30,  49,  13,  50,  49,  45,  13,  51,   5,  52,
     10,  49,  50,  53,   6,   1,  22,  15,  23,  54,  12,  39,  52,   5,  47,
     55,  56,  14,  57,  58,  46,  59,  60,  61,  62,  63,  64,  31,  17,  65,
     66,   4,   3,  20,  27,   7,  55,  57,  56,  55,  14,  45,  67,  47,  46,
     48,  22,  23,  26,  25,   4,  68,  69,  70,  28,  30,  24,  29,  48,  30,
     60,  71,  72,  58,  29,  28,  73,  63,  62,  67,  24,  27,  74,  75,  76,
     59,  71,  60,  47,   5,   4,  68,  77,  21,  78,  79,  40,  70,  80,  19,
     81,  73,  35,  44,  43,  82,  75,  83,  44,  52,  27,  20,  82,  43,  18,
     84,  85,  86,  50,  13,  12,  68,  53,  83,  68,  75,  74,  87,  88,  89,
     90,  61,  91,  92,  31,  65,  93,  33,  85,  87,  94,  16,  69,  72,  95,
     82,  32,  62,  80,  86,  85,  96,  20,  19,  19,  80,  97,  72,  71,  98,
     64,  91,  99, 100,  42, 101,  57,  46,  56,  99,  60,  72,  60,  99,  91,
     64,  99,  74, 102, 103,  93,  99,  72,  69,  80,  70,  95,  32,  82,  17,
     98, 104,  86,  81, 105,  63, 106, 107,  84, 108, 109, 105,  84,  86, 104,
    109,  64,  63, 106,  84, 104,  62,  32,  92,  65,  17,  16,  96,  89,  88,
    109, 110,  91,  90,  91, 110, 108, 110, 109, 111, 112, 113, 114, 115, 116,
    117, 118, 114,  94,  65,  16,  93, 103,  34, 119, 120, 121, 122, 123,  34,
     32,  31,  92,  73,  92,  94, 109,  63, 105, 124,  76,  82, 123, 125, 126,
    127, 128, 129, 122,  34, 103, 130,  78,  42, 102,  93, 107,  93,  85,  84,
     16,   3,  51,  87,  97,  85,  65,  94,  92,  35,  34, 123, 101,  42,  41,
    131, 132, 133, 134, 100, 115, 115, 101, 121, 135, 136, 137, 117, 113, 112,
    127, 129, 138, 139,  79,  78, 140, 141, 142, 120, 119, 118, 143, 139, 130,
    144, 118, 119, 144, 137, 136, 135, 143, 136, 112, 145, 146, 146, 129, 116,
    137, 144,  41, 146, 138, 129, 147, 135, 137,  79, 147,  40, 128, 148, 113,
    149, 150, 133,  40, 147, 137, 143, 130, 136, 136, 130, 100, 118, 144, 134,
    151, 148, 128, 146, 121, 120, 152, 153, 154, 113, 117, 155, 153, 131, 150,
    151, 128, 127, 152, 156, 131, 111, 145, 112, 157, 158, 159, 160, 161, 159,
    138, 146, 145, 148, 111, 113, 139,  78, 130, 155, 116, 129, 154, 153, 161,
    157, 162, 158, 163, 157, 159, 133, 132, 164, 165, 166, 167, 168, 169, 142,
    170, 171, 167, 172, 173, 174, 175, 163, 149, 176, 177, 178, 140, 154, 164,
    179, 180, 169, 154, 160, 181, 162, 174, 158, 182, 183, 142, 152, 180, 184,
    162, 172, 174, 179, 185, 180, 171, 186, 187, 149, 163, 159, 172, 175, 173,
    173, 175, 149, 182, 188, 156, 159, 161, 150, 141, 188, 182, 184, 189, 182,
    181, 160, 158, 185, 189, 184, 190, 191, 192, 141, 164, 132, 165, 177, 193,
    133, 181, 174, 189, 183, 182, 168, 179, 169, 190, 194, 178, 183, 168, 142,
    195, 196, 197, 185, 184, 180, 169, 180, 152, 198, 199, 200, 132, 131, 156,
    193, 177, 176, 201, 202, 203, 204, 205, 206, 190, 178, 187, 193, 200, 207,
    207, 208, 167, 209, 202, 195, 210, 211, 212, 213, 214, 212, 200, 215, 216,
    217, 191, 218, 196, 211, 210, 206, 205, 176, 212, 214, 205, 198, 214, 213,
    186, 190, 187, 216, 219, 208, 220, 192, 221, 216, 222, 219, 218, 186, 171,
    187, 178, 177, 199, 213, 223, 205, 214, 198, 224, 225, 197, 196, 221, 217,
    223, 226, 227, 190, 192, 220, 228, 211, 196, 229, 230, 231, 167, 208, 224,
    232, 233, 219, 218, 190, 186, 190, 234, 206, 192, 191, 217, 235,  36, 236,
    237, 238, 239, 227, 240, 215, 170, 241, 197, 227, 215, 200, 208, 219, 233,
    242, 222, 216, 108, 243, 244, 240, 227, 226, 204, 234, 220, 245, 246, 247,
    232, 219, 222, 201, 248, 202, 249, 245, 223, 209, 203, 202, 250, 239, 251,
    190, 220, 234, 245, 247, 223, 194, 190, 206, 233, 252, 209, 235, 253, 254,
    229, 255, 254, 212, 211, 228, 256, 235, 257, 246, 245, 248, 201, 246, 248,
    252, 203, 209, 226, 223, 247, 195, 202, 248, 249, 228, 248, 242, 216, 215,
    252, 233, 232, 106, 258, 238, 190, 218, 191, 225, 224, 233, 240, 242, 215,
    239, 238, 258, 253, 250, 251,  36, 259, 260, 230,  98,  71, 229, 258, 104,
    104,  98, 230, 231, 235, 255, 231, 230,  38,   2,   1,   6, 261, 126, 257,
    236, 260, 261, 125, 250, 253, 237, 239, 262, 122, 263, 123, 125, 263, 250,
     37, 235, 231, 122, 262, 263, 263, 262, 239, 255, 235, 254, 261, 243, 105,
    264, 244, 265, 251, 258, 229, 123, 263, 125, 106, 104, 258, 243, 260, 265,
      0,  44,  83, 260, 243, 261,  58,  28,  46, 235, 266, 257, 108, 105, 243,
     36, 235,  37, 235, 256, 253, 259, 265, 260, 126, 261, 105, 125, 256, 257,
    267,  38,  71, 235, 236, 266,  59, 267,  71, 264, 265, 268,  59, 268, 267,
    265, 259, 267,  10,  50,  11,   2,  11,  50, 267, 259,  38,  94,  87,  33,
     11,   2, 269,  23,  15, 269,   7,   6,  53,  66,  18,  43,  54,   0,   2,
     51,   3,   5, 269,   6,   8,  26,  12,  14,   3,  16,  18,  77,  19,  21,
     30,  23,  24,  56,  25,  14,  24,   8,  27, 259,  36,  38,  66,  39,  26,
     78,  40,  42,  39,  43,   0,  56,  46,  25,  96,  51,  52,  83,  53,   1,
      0,  54,  39,  67,  52,  47, 124,  62,  64,  18,  66,   3,  21,  20,   7,
     28,  67,  46,  66,  26,   4,  77,  68,  70,  67,  28,  24,  92,  73,  62,
     52,  67,  27, 124,  74,  76,  25,  47,   4,  53,  68,  21,  77,  70,  19,
    123,  81,  35,  76,  44,  82,  76,  75,  44,  96,  52,  20,  17,  82,  18,
     54,  50,  12,  75,  68,  83,  69,  68,  74,  97,  87,  89,  88,  87,  16,
     70,  69,  95,  97,  80,  85,  89,  96,  19,  89,  19,  97,  95,  72,  98,
    115, 100, 101,  61,  60,  91, 124,  64,  74,  74,  99,  69,  86,  80,  95,
     95,  98,  86,  73,  81,  63,  51,  96,  88,  64, 109,  91, 155, 114, 116,
    155, 117, 114,  33,  93,  34, 101, 119, 121,  35,  73,  94,  62, 124,  82,
     81, 123, 126, 100, 130,  42, 107,  93,  84,  88,  16,  51,  33,  87,  85,
    119, 101,  41, 150, 131, 133, 114, 134, 115, 116, 115, 121, 120, 117, 112,
    169, 140, 142, 117, 120, 118,  41, 144, 119, 134, 144, 136, 121, 146, 116,
     40, 137,  41, 173, 149, 133, 134, 136, 100, 114, 118, 134, 112, 146, 120,
    140, 152, 154, 128, 113, 155, 161, 153, 150, 153, 152, 131, 158, 160, 159,
    128, 155, 129, 160, 154, 161, 181, 133, 164, 171, 165, 167, 241, 170, 167,
    194, 176, 178, 141, 140, 164, 164, 154, 181, 156, 152, 184, 165, 171, 187,
    184, 182, 156, 149, 159, 150, 142, 141, 182, 174, 181, 158, 188, 141, 132,
    166, 165, 193, 173, 133, 174, 225, 195, 197, 140, 169, 152, 193, 198, 200,
    188, 132, 156, 198, 193, 176, 234, 204, 206, 166, 193, 207, 166, 207, 167,
    225, 209, 195, 204, 210, 212, 249, 213, 212, 207, 200, 216, 170, 217, 218,
    221, 196, 210, 194, 206, 176, 204, 212, 205, 199, 198, 213, 207, 216, 208,
    210, 220, 221, 170, 218, 171, 165, 187, 177, 227, 199, 223, 176, 205, 198,
    241, 224, 197, 197, 196, 217, 195, 228, 196, 255, 229, 231, 241, 167, 224,
    221, 192, 217, 217, 170, 197, 199, 227, 200, 224, 208, 233, 210, 204, 220,
    213, 249, 223, 251, 229, 254, 249, 212, 228, 228, 195, 248, 245, 249, 248,
    209, 225, 233, 251, 239, 258, 254, 253, 251, 236,  36, 260,  38, 230,  71,
    229, 104, 230,  37, 231,  38, 269,   2,   6, 266, 261, 257, 266, 236, 261,
    256, 125, 253, 250, 263, 239, 244, 243, 265,   1,   0,  83,  81, 126, 105,
    126, 125, 257, 268, 265, 267,  54,   2,  50,  35,  94,  33,  15,  11, 269,
      8,  23, 269,  21,   7,  53,  39,  66,  43
};

int teapotNormalIndicesLength = sizeof(teapotNormalIndices) / sizeof(int);

//
// makeTeapot() - create a teapot object
//
static void makeTeapot( Canvas &C )
{
    int i;

    for( i = 0; i < teapotElementsLength - 2; i += 3 ) {

        // Calculate the base indices of the three vertices
        int point1 = 3 * teapotElements[i];     // slots 0, 1, 2
        int point2 = 3 * teapotElements[i + 1]; // slots 3, 4, 5
        int point3 = 3 * teapotElements[i + 2]; // slots 6, 7, 8

        Vertex p1 = { teapotVertices[point1 + 0],
                      teapotVertices[point1 + 1],
                      teapotVertices[point1 + 2] };

        Vertex p2 = { teapotVertices[point2 + 0],
                      teapotVertices[point2 + 1],
                      teapotVertices[point2 + 2] };

        Vertex p3 = { teapotVertices[point3 + 0],
                      teapotVertices[point3 + 1],
                      teapotVertices[point3 + 2] };

        // Calculate the base indices of the three normals
        int normal1 = 3 * teapotNormalIndices[i];     // slots 0, 1, 2
        int normal2 = 3 * teapotNormalIndices[i + 1]; // slots 3, 4, 5
        int normal3 = 3 * teapotNormalIndices[i + 2]; // slots 6, 7, 8

        Normal n1 = { teapotNormals[normal1 + 0],
                      teapotNormals[normal1 + 1],
                      teapotNormals[normal1 + 2] };

        Normal n2 = { teapotNormals[normal2 + 0],
                      teapotNormals[normal2 + 1],
                      teapotNormals[normal2 + 2] };

        Normal n3 = { teapotNormals[normal3 + 0],
                      teapotNormals[normal3 + 1],
                      teapotNormals[normal3 + 2] };

        // Add triangle and vertex normals
        C.addTriangleWithNorms( p1, n1, p2, n2, p3, n3 );

    }
}

//
// The quad is parallel to the XY plane with the front face
// pointing down the +Z axis
//

//
// Each group of three values specifies a quad vertex
//
float quadVertices[] = {
    // top row
    -0.50,  0.50,  0.50, -0.25,  0.50,  0.50,  0.00,  0.50,  0.50,
     0.25,  0.50,  0.50,  0.50,  0.50,  0.50,
    // second row
    -0.50,  0.25,  0.50, -0.25,  0.25,  0.50,  0.00,  0.25,  0.50,
     0.25,  0.25,  0.50,  0.50,  0.25,  0.50,
    // third (middle) row
    -0.50,  0.00,  0.50, -0.25,  0.00,  0.50,  0.00,  0.00,  0.50,
     0.25,  0.00,  0.50,  0.50,  0.00,  0.50,
    // fourth row
    -0.50, -0.25,  0.50, -0.25, -0.25,  0.50,  0.00, -0.25,  0.50,
     0.25, -0.25,  0.50,  0.50, -0.25,  0.50,
    // fifth (last) row
    -0.50, -0.50,  0.50, -0.25, -0.50,  0.50,  0.00, -0.50,  0.50,
     0.25, -0.50,  0.50,  0.50, -0.50,  0.50
};

int quadVerticesLength = sizeof(quadVertices) / sizeof(float);

//
// Each pair of values specifies a vertex's texture coordinates
//

float quadUV[] = {
    // top row
     0.00, 1.00, 0.25, 1.00, 0.50, 1.00, 0.75, 1.00, 1.00, 1.00,
    // second row
     0.00, 0.75, 0.25, 0.75, 0.50, 0.75, 0.75, 0.75, 1.00, 0.75,
    // third (middle) row
     0.00, 0.50, 0.25, 0.50, 0.50, 0.50, 0.75, 0.50, 1.00, 0.50,
    // fourth row
     0.00, 0.25, 0.25, 0.25, 0.50, 0.25, 0.75, 0.25, 1.00, 0.25,
    // fifth (last) row
     0.00, 0.00, 0.25, 0.00, 0.50, 0.00, 0.75, 0.00, 1.00, 0.00
};

//
// Because the quad faces +Z, all the normals are (0,0,1)
//
float quadNormals[] = { 0.0, 0.0, 1.0 };

int quadNormalsLength = sizeof(quadNormals) / sizeof(float);

//
// Each group of three entries specifies a triangle for the quad
//
int quadElements[] = {
    // top row
     0,  5,  1,  5,  6,  1,  1,  6,  2,  6,  7,  2,
     2,  7,  3,  7,  8,  3,  3,  8,  4,  8,  9,  4,
    // second row
     5, 10,  6, 10, 11,  6,  6, 11,  7, 11, 12,  7,
     7, 12,  8, 12, 13,  8,  8, 13,  9, 13, 14,  9,
    // third row
    10, 15, 11, 15, 16, 11, 11, 16, 12, 16, 17, 12,
    12, 17, 13, 17, 18, 13, 13, 18, 14, 18, 19, 14,
    // fourth row
    15, 20, 16, 20, 21, 16, 16, 21, 17, 21, 22, 17,
    17, 22, 18, 22, 23, 18, 18, 23, 19, 23, 24, 19
};

int quadElementsLength = sizeof(quadElements) / sizeof(int);

//
// makeQuad() - create a quad object
//
static void makeQuad( Canvas &C )
{
    int i;

    for( i = 0; i < quadElementsLength - 2; i += 3 ) {

        // Calculate the base indices of the three vertices
        int point1 = 3 * quadElements[i];     // slots 0, 1, 2
        int point2 = 3 * quadElements[i + 1]; // slots 3, 4, 5
        int point3 = 3 * quadElements[i + 2]; // slots 6, 7, 8

        Vertex p1 = { quadVertices[point1 + 0],
                      quadVertices[point1 + 1],
                      quadVertices[point1 + 2] };

        Vertex p2 = { quadVertices[point2 + 0],
                      quadVertices[point2 + 1],
                      quadVertices[point2 + 2] };

        Vertex p3 = { quadVertices[point3 + 0],
                      quadVertices[point3 + 1],
                      quadVertices[point3 + 2] };

        // Calculate the base indices of the three texture coordinates
        int normal1 = 2 * quadElements[i];     // slots 0, 1, 2
        int normal2 = 2 * quadElements[i + 1]; // slots 3, 4, 5
        int normal3 = 2 * quadElements[i + 2]; // slots 6, 7, 8

        // UVcoord is really Vertex, so we need a 'z' component
        UVcoord uv1 = { quadUV[normal1 + 0], quadUV[normal1 + 1], 0.0f };
        UVcoord uv2 = { quadUV[normal2 + 0], quadUV[normal2 + 1], 0.0f };
        UVcoord uv3 = { quadUV[normal3 + 0], quadUV[normal3 + 1], 0.0f };

        // Add triangle and texture coordinates
        C.addTriangleWithUV( p1, uv1, p2, uv2, p3, uv3 );
    }
}

///
// Make either a quad or teapot
//
// @param choice - 0 for quad. 1 for teapot
// @param C      - the Canvas we'll use
///
void makeShape( int choice, Canvas &C )
{
    switch(choice){
		case OBJ_TEAPOT 	: makeTeapot(C);
			break;
		case OBJ_QUAD 		: makeQuad(C);
			break;
		case OBJ_CUBE 		: makeCube2(C);
			break;
		case OBJ_CYLINDER 	: makeCylinder(C, 1, 40, 3);
			break;
		case OBJ_CONE 		: makeCone(C, 1, 90, 3);
			break;
		case OBJ_SPHERE 	: makeSphere(C, 1, 30, 30);
			break;
		case OBJ_PEAR	: makeSphere(C, 1, 6, 5);
			break;
		
	}
}
