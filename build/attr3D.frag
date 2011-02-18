varying vec3 normal;
varying vec3 trueNormal;
varying vec4 position;

uniform int xAttr;
uniform float xMod;
uniform float xZMin;
uniform int xFlip;

uniform int yAttr;
uniform float yMod;
uniform float yZMin;
uniform int yFlip;

uniform int zAttr;
uniform float zMod;
uniform float zZMin;
uniform int zFlip;

uniform int numTex;
uniform float texPos[3];
uniform float texHSkew[3];
uniform float texVSkew[3];
uniform int texInterp[3];

uniform int autoDiffuse;
uniform int autoSpecular;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;

vec3 view;
vec3 light;
vec3 reflection;

float sphereReflectp;
float sphereMapp;
bool didsetReflectp = false;
bool didsetMapp = false;

int prevTex;
int nextTex;
float prevTexPos;
float nextTexPos;

const float PI = 3.1415926535;

void main() {

	view = normalize(position.xyz);
	light = normalize((position-gl_LightSource[0].position).xyz);
	reflection = normalize(reflect(view, normal));
	
	float xAttrVal;
	float yAttrVal;
	float zAttrVal;
	
	// X Attribute
	if (xAttr == 0) {
		float kdiff = -dot(light, normal);
		kdiff = max(kdiff, 0.0);
		kdiff = pow(kdiff, xMod);
		xAttrVal = (kdiff);
	} else if (xAttr == 1) {
		float kdiff = (-dot(light, normal)+1.0)/2.0;
		kdiff = max(kdiff, 0.0);
		kdiff = pow(kdiff, xMod);
		xAttrVal = (kdiff);
	} else if (xAttr == 2) {
		float kspec =-dot(reflection,light);
		kspec = max(kspec, 0.0);
		kspec = pow(kspec, xMod);
		xAttrVal = (kspec);
	} else if (xAttr == 3) {
		vec3 refraction = normalize(refract(view, normal, xZMin));
		float krefr =-dot(refraction,light);
		krefr = max(krefr, 0.0);
		krefr = pow(krefr, xMod);
		xAttrVal = (krefr);
	} else if (xAttr == 4) {
		float korient = abs(dot(view, normal));
		korient = pow(korient, xMod);
		xAttrVal = (korient);
	} else if (xAttr == 5) {
		xAttrVal = (log((-position.z)/xZMin)/log(xMod));
	} else if (xAttr == 6) {
		float light_dist = length((position-gl_LightSource[0].position).xyz);
		xAttrVal = (log(2.0*light_dist/xZMin)/log(xMod));
	} else if (xAttr == 7) {
		vec3 camera = vec3(0.0, 0.0, 1.0);
		float kcam = abs(dot(view, camera));
		kcam = max(kcam, 0.0);
		kcam = pow(kcam, xMod*10.0);
		xAttrVal = (kcam);
	} else if (xAttr == 8) {
		if (!didsetReflectp) {
			sphereReflectp = sqrt(pow(reflection.x, 2.0) + pow(reflection.y, 2.0) + pow(reflection.z+1.0, 2.0));
			didsetReflectp = true;
		}
		xAttrVal = (reflection.x/(2.0*sphereReflectp)+0.5);
	} else if (xAttr == 9) {
		if (!didsetReflectp) {
			sphereReflectp = sqrt(pow(reflection.x, 2.0) + pow(reflection.y, 2.0) + pow(reflection.z+1.0, 2.0));
			didsetReflectp = true;
		}
		xAttrVal = (reflection.y/(2.0*sphereReflectp)+0.5);
	} else if (xAttr == 10) {
		float c_angle = (atan(trueNormal.x, trueNormal.z)+PI)/PI/2.0;
		xAttrVal = mod(c_angle+xMod*0.1-0.1, 1.0);
	} else if (xAttr == 11) {
		float c_angle;
		if (trueNormal.y >= 0.0)
			c_angle = (1.0-acos(trueNormal.y)/PI);
		else
			c_angle = (acos(-trueNormal.y)/PI);
			
		xAttrVal = mod(c_angle+xMod*0.1-0.1, 1.0);
	}
	else
		xAttrVal = (0.0);
				
	// Y Attribute
	if (yAttr == 0) {
		float kdiff = -dot(light, normal);
		kdiff = max(kdiff, 0.0);
		kdiff = pow(kdiff, yMod);
		yAttrVal = (kdiff);
	} else if (yAttr == 1) {
		float kdiff = (-dot(light, normal)+1.0)/2.0;
		kdiff = max(kdiff, 0.0);
		kdiff = pow(kdiff, yMod);
		yAttrVal = (kdiff);
	} else if (yAttr == 2) {
		float kspec =-dot(reflection,light);
		kspec = max(kspec, 0.0);
		kspec = pow(kspec, yMod);
		yAttrVal = (kspec);
	} else if (yAttr == 3) {
		vec3 refraction = normalize(refract(view, normal, yZMin));
		float krefr =-dot(refraction,light);
		krefr = max(krefr, 0.0);
		krefr = pow(krefr, yMod);
		yAttrVal = (krefr);
	} else if (yAttr == 4) {
		float korient = abs(dot(view, normal));
		korient = pow(korient, yMod);
		yAttrVal = (korient);
	} else if (yAttr == 5) {
		yAttrVal = (log((-position.z)/yZMin)/log(yMod));
	} else if (yAttr == 6) {
		float light_dist = length((position-gl_LightSource[0].position).xyz);
		yAttrVal = (log(2.0*light_dist/yZMin)/log(yMod));
	} else if (yAttr == 7) {
		vec3 camera = vec3(0.0, 0.0, 1.0);
		float kcam = abs(dot(view, camera));
		kcam = max(kcam, 0.0);
		kcam = pow(kcam, yMod*10.0);
		yAttrVal = (kcam);
	} else if (yAttr == 8) {
		if (!didsetReflectp) {
			sphereReflectp = sqrt(pow(reflection.x, 2.0) + pow(reflection.y, 2.0) + pow(reflection.z+1.0, 2.0));
			didsetReflectp = true;
		}
		yAttrVal = (reflection.x/(2.0*sphereReflectp)+0.5);
	} else if (yAttr == 9) {
		if (!didsetReflectp) {
			sphereReflectp = sqrt(pow(reflection.x, 2.0) + pow(reflection.y, 2.0) + pow(reflection.z+1.0, 2.0));
			didsetReflectp = true;
		}
		yAttrVal = (reflection.y/(2.0*sphereReflectp)+0.5);
	} else if (yAttr == 10) {
		float c_angle = (atan(trueNormal.x, trueNormal.z)+PI)/PI/2.0;
		yAttrVal = mod(c_angle+yMod*0.1-0.1, 1.0);
	} else if (yAttr == 11) {
		float c_angle;
		if (trueNormal.y >= 0.0)
			c_angle = (1.0-acos(trueNormal.y)/PI);
		else
			c_angle = (acos(-trueNormal.y)/PI);
			
		yAttrVal = mod(c_angle+yMod*0.1-0.1, 1.0);
	}
	else
		yAttrVal = (0.0);
		
	// Z Attribute
	if (zAttr == 0) {
		float kdiff = -dot(light, normal);
		kdiff = max(kdiff, 0.0);
		kdiff = pow(kdiff, zMod);
		zAttrVal = (kdiff);
	} else if (zAttr == 1) {
		float kdiff = (-dot(light, normal)+1.0)/2.0;
		kdiff = max(kdiff, 0.0);
		kdiff = pow(kdiff, zMod);
		zAttrVal = (kdiff);
	} else if (zAttr == 2) {
		float kspec =-dot(reflection,light);
		kspec = max(kspec, 0.0);
		kspec = pow(kspec, zMod);
		zAttrVal = (kspec);
	} else if (zAttr == 3) {
		vec3 refraction = normalize(refract(view, normal, zZMin));
		float krefr =-dot(refraction,light);
		krefr = max(krefr, 0.0);
		krefr = pow(krefr, zMod);
		zAttrVal = (krefr);
	} else if (zAttr == 4) {
		float korient = abs(dot(view, normal));
		korient = pow(korient, zMod);
		zAttrVal = (korient);
	} else if (zAttr == 5) {
		zAttrVal = (log((-position.z)/zZMin)/log(zMod));
	} else if (zAttr == 6) {
		float light_dist = length((position-gl_LightSource[0].position).xyz);
		zAttrVal = (log(2.0*light_dist/zZMin)/log(zMod));
	} else if (zAttr == 7) {
		vec3 camera = vec3(0.0, 0.0, 1.0);
		float kcam = abs(dot(view, camera));
		kcam = max(kcam, 0.0);
		kcam = pow(kcam, zMod*10.0);
		zAttrVal = (kcam);
	} else if (zAttr == 8) {
		if (!didsetReflectp) {
			sphereReflectp = sqrt(pow(reflection.x, 2.0) + pow(reflection.y, 2.0) + pow(reflection.z+1.0, 2.0));
			didsetReflectp = true;
		}
		zAttrVal = (reflection.x/(2.0*sphereReflectp)+0.5);
	} else if (zAttr == 9) {
		if (!didsetReflectp) {
			sphereReflectp = sqrt(pow(reflection.x, 2.0) + pow(reflection.y, 2.0) + pow(reflection.z+1.0, 2.0));
			didsetReflectp = true;
		}
		zAttrVal = (reflection.y/(2.0*sphereReflectp)+0.5);
	} else if (zAttr == 10) {
		float c_angle = (atan(trueNormal.x, trueNormal.z)+PI)/PI/2.0;
		zAttrVal = mod(c_angle+zMod*0.1-0.1, 1.0);
	} else if (zAttr == 11) {
		float c_angle;
		if (trueNormal.y >= 0.0)
			c_angle = (1.0-acos(trueNormal.y)/PI);
		else
			c_angle = (acos(-trueNormal.y)/PI);
			
		zAttrVal = mod(c_angle+zMod*0.1-0.1, 1.0);
	}
	else
		zAttrVal = (0.0);
	
	//Set x and y position to x and y attributes
	xAttrVal = min(xAttrVal, 1.0);
	xAttrVal = max(xAttrVal, 0.0);
	yAttrVal = min(yAttrVal, 1.0);
	yAttrVal = max(yAttrVal, 0.0);
	zAttrVal = min(zAttrVal, 1.0);
	zAttrVal = max(zAttrVal, 0.0);
	
	vec2 texcoord = vec2(xAttrVal, yAttrVal);
	if (xFlip == 1)
		texcoord.x = 1.0-texcoord.x;
	if (yFlip == 1)
		texcoord.y = 1.0-texcoord.y;

	//Set z position to z attribute
	float zPos = zAttrVal;
	
	if (zFlip == 1)
		zPos = 1.0-zPos;
	
	//Find the textures adjacent to the attribute point
	prevTex = -1;
	nextTex = -1;
	
	float prevDist = 2.0;
	float nextDist = 2.0;
	
	int i = 0; // This is an unrolled loop. See other shader for properly coded version
	
		if (i < numTex) {
		
			float tpos = (texPos[i] + (texcoord.x-0.5)*(texHSkew[i]) + (texcoord.y-0.5)*(texVSkew[i]));
			float dist = tpos-zPos;
			if (dist < 0.0) {
				if (-dist < prevDist) {
					prevDist = -dist;
					prevTex = i;
					prevTexPos = tpos;
				}
			}
			else {
				if (dist < nextDist) {
					nextDist = dist;
					nextTex = i;
					nextTexPos = tpos;
				}
			}
		}
	i = 1;
		if (i < numTex) {
		
			float tpos = (texPos[i] + (texcoord.x-0.5)*(texHSkew[i]) + (texcoord.y-0.5)*(texVSkew[i]));
			float dist = tpos-zPos;
			if (dist < 0.0) {
				if (-dist < prevDist) {
					prevDist = -dist;
					prevTex = i;
					prevTexPos = tpos;
				}
			}
			else {
				if (dist < nextDist) {
					nextDist = dist;
					nextTex = i;
					nextTexPos = tpos;
				}
			}
		}
	i = 2;
		if (i < numTex) {
		
			float tpos = (texPos[i] + (texcoord.x-0.5)*(texHSkew[i]) + (texcoord.y-0.5)*(texVSkew[i]));
			float dist = tpos-zPos;
			if (dist < 0.0) {
				if (-dist < prevDist) {
					prevDist = -dist;
					prevTex = i;
					prevTexPos = tpos;
				}
			}
			else {
				if (dist < nextDist) {
					nextDist = dist;
					nextTex = i;
					nextTexPos = tpos;
				}
			}
		}

	//For cases where there is only one adjacent texture, use that texture color
	vec4 nextTexColour;
	vec4 prevTexColour;
	
	if (prevTex == 0)
		prevTexColour = (texture2D(tex0, texcoord));
	else if (prevTex == 1)
		prevTexColour = (texture2D(tex1, texcoord));
	else if (prevTex == 2)
		prevTexColour = (texture2D(tex2, texcoord));
	else if (prevTex == 3)
		prevTexColour = (texture2D(tex3, texcoord));
	else
		prevTexColour = (vec4(1.0, 1.0, 1.0, 1.0));
		
	if (nextTex == 0)
		nextTexColour = (texture2D(tex0, texcoord));
	else if (nextTex == 1)
		nextTexColour = (texture2D(tex1, texcoord));
	else if (nextTex == 2)
		nextTexColour = (texture2D(tex2, texcoord));
	else if (nextTex == 3)
		nextTexColour = (texture2D(tex3, texcoord));
	else
		nextTexColour = (vec4(1.0, 1.0, 1.0, 1.0));
		
	if (prevTex == -1 && nextTex != -1) {
		gl_FragColor = nextTexColour;
	}
	else if (prevTex != -1 && nextTex == -1) {
		gl_FragColor = prevTexColour;
	}
	else if (prevTex != -1 && nextTex != -1) {
		//Interpolate between the two textures based on location between them
		float u = (zPos-prevTexPos)/(nextTexPos-prevTexPos);
		gl_FragColor = prevTexColour*(1.0-u)+nextTexColour*u;
	}
	else {
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
	
	//Apply auto-shading techniques and ambient light
	if (autoDiffuse == 1) {
		float kdiff = -dot(light, normal);
		kdiff = max(kdiff, 0.0);
		gl_FragColor = gl_FragColor*kdiff;
	}
	if (autoSpecular == 1) {
		float kspec =-dot(reflection,light);
		kspec = max(kspec, 0.0);
		kspec = pow(kspec, 7.0);
		gl_FragColor = gl_FragColor+kspec*gl_LightSource[0].specular;
	}

	vec4 texValue = texture2D(tex3,gl_TexCoord[0].st);
	float balance = 0.3;
	if (xAttr == 5) balance = balance * (1.0-xAttrVal);
	if (yAttr == 5) balance = balance * (1.0-yAttrVal);
	if (zAttr == 5) balance = balance * (1.0-zAttrVal);
	gl_FragColor = (1.1 - balance)*(gl_FragColor + gl_LightSource[0].ambient) + balance * texValue;
}
