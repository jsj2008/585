#pragma once

#include <stdlib.h>
#include <math.h>
#include <LinearMath/btIDebugDraw.h>
#include <vector>

using namespace std;

class Face {
	public:
		Face();
		~Face();

		void addVertex(int v, int vt, int vn);
		bool containsIndex(int v);

		vector<int> vertices;	// Indices of the vertices in the face
		vector<int> texCoords;	// Indices of the texture coordinates in the face
		vector<int> normals;	// Indices of the normals in the face
};
