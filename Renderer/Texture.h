#pragma once

#include <string>
#include <math.h>
using namespace std;

class Texture
{
public:
	Texture();
	~Texture();

	float minPos();
	float maxPos();

	static float minPos(float pos, float h_skew, float v_skew);
	static float maxPos(float pos, float h_skew, float v_skew);

	float vertexPos(int vertex);
	static float vertexPos(int vertex, float pos, float h_skew, float v_skew);

	string filename;
	float pos;
	float h_skew;
	float v_skew;
	bool interpolate;
};
