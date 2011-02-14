// basic vertex shader

varying vec3 normal;
varying vec3 trueNormal;
varying vec4 position;

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	normal = normalize(gl_NormalMatrix * gl_Normal);
	//mat3 w = mat3(gl_ModelViewMatrix);
	trueNormal = normalize(gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	position = gl_ModelViewMatrix * gl_Vertex;

}
