varying vec3 trueNormal;

void main() {	
	trueNormal = normalize(gl_Normal);
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
