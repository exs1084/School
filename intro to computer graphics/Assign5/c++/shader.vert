//
// Alternate vertex shader for the transformation assignment
//
// Created by Warren R. Carithers 2016/04/22.
//
// Contributor:  Erikson Sodergren
//

#version 120

// attribute:  vertex position
attribute vec4 vPosition;

// add other global shader variables to hold the
// parameters your application code is providing

uniform vec3 theta;
uniform vec3 scales;
uniform vec3 trans;
uniform vec3 eye;
uniform vec3 lookat;
uniform vec3 up;
//dont know what vars are in vec6, so using 2 vecs.
uniform vec3 proj1;//left, right, near
uniform vec3 proj2;//bottom, top, far
uniform bool ortho;

void main()
{
    // By default, no transformations are performed.
    vec3 angles = radians( theta );
    vec3 c = cos( angles );
    vec3 s = sin( angles );
    
    
    // rotation matrices
    mat4 rx = mat4 ( 1.0,  0.0,  0.0,  0.0,
                     0.0,  c.x,  s.x,  0.0,
                     0.0, -s.x,  c.x,  0.0,
                     0.0,  0.0,  0.0,  1.0 );

    mat4 ry = mat4 ( c.y,  0.0, -s.y,  0.0,
                     0.0,  1.0,  0.0,  0.0,
                     s.y,  0.0,  c.y,  0.0,
                     0.0,  0.0,  0.0,  1.0 );

    mat4 rz = mat4 ( c.z,  s.z,  0.0,  0.0,
                    -s.z,  c.z,  0.0,  0.0,
                     0.0,  0.0,  1.0,  0.0,
                     0.0,  0.0,  0.0,  1.0 );
                     
    //translation/scaling matrix/matrices
    mat4 scale = mat4(scales.x, 0.0, 0.0, 0.0,
					  0.0, scales.y, 0.0, 0.0,
					  0.0, 0.0, scales.z, 0.0,
					  0.0, 0.0, 0.0, 1.0);
					  
	mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
					  0.0, 1.0, 0.0, 0.0,
					  0.0, 0.0, 1.0, 0.0,
					  trans.x, trans.y, trans.z, 1.0);
    
    mat4 modelMat = trans * ry * rz * rx * scale;//product of all model transformations
    //camera transformations
    vec3 n = normalize(eye-lookat);
    vec3 u = normalize(cross(up, n));
    vec3 v = normalize(cross(n, u));
    
                      
    mat4 viewMat = mat4 ( u.x,  v.x,  n.x,  0.0,//view
						  u.y,  v.y,  n.y,  0.0,
						  u.z,  v.z,  n.z,  0.0,
          (-1*dot(u, eye)),  (-1*dot(v, eye)),  (-1*dot(n, eye)),  1.0 );
    
    //projection matrix
    mat4 projMat;
    float r, l, t, b, near, f;
    l=proj1.x, r=proj1.y, near=proj1.z;
    b=proj2.x, t=proj2.y, f=proj2.z;
    if(ortho){
		projMat = mat4 (2/(r-l),  	0,  		0, 			  0,
					    0,			2/(t-b), 	0, 			  0,
					    0,  		0, 			-2/(f-near),  0,
					(-r-l)/(r-l), (-t-b)/(t-b), (-f-near)/(f-near), 1.0);//projection
	}else{
		projMat = mat4 (
		2*near/(r-l), 	0,  		 	 0, 				 0,
		0, 	 			2*near/(t-b),	 0,  				 0,
		(r+l)/(r-l), 	(t+b)/(t-b), 	 (-f-near)/(f-near), -1.0,
		0, 		 		0, 				 -2*f*near/(f-near), 0);//projection
	}
    gl_Position = projMat * viewMat * modelMat * vPosition;
    //gl_Position.z*=0.2;//only actually looks like a teapot with this?
}
