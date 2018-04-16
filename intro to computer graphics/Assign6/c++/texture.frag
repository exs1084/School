#version 130

// Texture mapping vertex shader
//
// Contributor:  Erikson Sodergren

// INCOMING DATA

// Here is where you should add the variables you need in order
// to propogate data from the vertex shader to the fragment shader
// so that it can do the shading and texture mapping computations
in vec3 lightInCamera;
in vec3 vertexInCamera;
in vec3 N;
in vec2 texCoord;

uniform float ka;
uniform float kd;
uniform float ks;
uniform float specExp;
uniform vec4 lightColor;
uniform vec4 ambient;

uniform sampler2D smile;
uniform sampler2D frown;
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
    vec4 myAmbient;
    
    
    
    
    
    
    
    vec4 face;
    if(gl_FrontFacing){//which face?
		face = texture(smile, texCoord);
		
	}else{
		face = texture(frown, texCoord);
		
	}
	
	
	
	
	
	
	
	
    vec4 amb  = ambient * ka * face;
    vec4 diff = lightColor * kd * face * cosTheta;
    vec4 spec = lightColor * ks * face * pow(cosAlpha, specExp);
    //finalColor = vec4(.2, .3, .4, 1);
    finalColor = amb + diff + spec;//+texture pixel
    //finalColor = texture(smile, texCoord);
}
