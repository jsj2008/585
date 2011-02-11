#include "Texture.h"

Texture::Texture() {
	pos = 0.0f;
	h_skew = 0.0f;
	v_skew = 0.0f;
	interpolate = true;
}

Texture::~Texture() { }

float Texture::vertexPos(int vertex) {
	return vertexPos(vertex, pos, h_skew, v_skew);
}

float Texture::vertexPos(int vertex, float pos, float h_skew, float v_skew) {
	if (vertex < 0 || vertex > 3)
		return 0.0f;

	float h_factor = 0.0f;
	float v_factor = 0.0f;

	if (vertex == 0) {
		h_factor =-1.0f;
		v_factor =-1.0f;
	}
	else if (vertex == 1) {
		h_factor =-1.0f;
		v_factor = 1.0f;
	}
	else if (vertex == 2) {
		h_factor = 1.0f;
		v_factor = 1.0f;
	}
	else if (vertex == 3) {
		h_factor = 1.0f;
		v_factor =-1.0f;
	}

	return (pos + h_skew*h_factor*0.5f + v_skew*v_factor*0.5f);
}

float Texture::minPos() {
	return minPos(pos, h_skew, v_skew);
}

float Texture::maxPos() {
	return maxPos(pos, h_skew, v_skew);
}

float Texture::minPos(float pos, float h_skew, float v_skew) {
	float min = 2.0f;
	for (int i = 0; i < 4; i++) {
		float v_pos = vertexPos(i, pos, h_skew, v_skew);
		if (v_pos < min)
			min = v_pos;
	}
	return min;
}

float Texture::maxPos(float pos, float h_skew, float v_skew) {
	float max = -1.0f;
	for (int i = 0; i < 4; i++) {
		float v_pos = vertexPos(i, pos, h_skew, v_skew);
		if (v_pos > max)
			max = v_pos;
	}
	return max;
}