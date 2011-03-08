#pragma once

#include <string>
#include <math.h>
using namespace std;

class Texture {
public:
	Texture();
	~Texture();

	float minPos();
	float maxPos();

	static float minPos(float pos, float h_skew, float v_skew);
	static float maxPos(float pos, float h_skew, float v_skew);

	float vertexPos(int vertex);
	static float vertexPos(int vertex, float pos, float h_skew, float v_skew);

	string filename;	// Original name of the texture file
	float pos;			// Texture position in the 3D texture
	float h_skew;		// Horizontal skew in the 3D texture
	float v_skew;		// Vertical skew in the 3D texture
	bool interpolate;	// Whether or not to interpolate with the next texture. Not in use due to shader compatability issues
};
