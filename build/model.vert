varying vec3 normal;
varying vec3 lightDir;
varying vec3 viewDir;
varying vec3 trueNormal;
varying vec4 position;
attribute vec3 vertTangent; 

void main() {	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	
	normal = normalize(gl_NormalMatrix * gl_Normal);
	vec3 tangent = normalize(gl_NormalMatrix * vertTangent);
	vec3 binormal = cross(normal, tangent);
	trueNormal = normalize(gl_Normal);
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	position = gl_ModelViewMatrix * gl_Vertex;
	
	vec3 temp = gl_LightSource[0].position.xyz - position.xyz;

	lightDir.x = dot(temp, tangent);
	lightDir.y = dot(temp, binormal);
	lightDir.z = dot(temp, normal);

	temp = -position.xyz;
	viewDir.x = dot(temp, tangent);
	viewDir.y = dot(temp, binormal);
	viewDir.z = dot(temp, normal);
}
