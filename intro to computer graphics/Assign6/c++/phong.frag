#version 130

// Phong fragment shader
//
// Contributor:  Erikson Sodergren

// INCOMING DATA

// Here is where you should add the variables you need in order
// to propogate data from the vertex shader to the fragment shader
// so that it can do the shading computations
in vec3 lightInCamera;
in vec3 vertexInCamera;
in vec3 N;

uniform vec4 Oa;
uniform vec4 Od;
uniform vec4 Os;
uniform float ka;
uniform float kd;
uniform float ks;
uniform float specExp;
uniform vec4 lightColor;
uniform vec4 ambient;
// OUTGOING DATA
out vec4 finalColor;

void main()
{
		
	vec3 L = normalize(lightInCamera - vertexInCamera);
	vec3 R = reflect(-L, N);//reflected
	vec3 V = normalize(-vertexInCamera);
	
	float cosAlpha = max(dot(R, V), 0.0);
	float cosTheta = max(dot(L, N), 0.0);
	
    // Replace this code with your implementation of this shader
    vec4 amb  = ambient * ka * Oa;
    vec4 diff = lightColor * kd * Od * cosTheta;
    vec4 spec = lightColor * ks * Os * pow(cosAlpha, specExp);
    finalColor = amb + diff + spec;
    //finalColor = spec;
}
