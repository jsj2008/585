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

		vector<int> vertices;
		vector<int> texCoords;
		vector<int> normals;
};
