//attribute vec3 vertex;
//attribute vec3 normal;
varying vec3 position, normal;
attribute vec2 uv1;
attribute vec4 tangent;

uniform mat4 _mv; // model-view matrix
uniform mat4 _mvProj; // model-view-projection matrix
uniform mat3 _norm; // normal matrix
uniform float _time; // time in seconds

varying vec2 uvV;
varying vec3 n;

varying float randomOffset;
float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


void main(void) {
	// compute position
//	gl_Position = gl_ModelViewProjectionMatrix * vec4(vertex, 1.0);
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	position = gl_Vertex.xyz;
	normal = gl_Normal.xyz;
	randomOffset = rand(gl_Vertex.xy + gl_Vertex.yz);

	uvV = uv1;
	//randomOffset = rand(vertex.xy + vertex.yz);
	// compute light info
	n = normalize(_norm * normal);
}