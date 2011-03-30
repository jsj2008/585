varying vec3 trueNormal;

uniform sampler2D texMap;

void main() {
	//float uppity = dot(trueNormal, vec3(0,-1,0));
	//gl_FragColor = vec4(1.5-uppity,1.5-uppity,1,1);
	gl_FragColor = texture2D(texMap, gl_TexCoord[0].st);
}
