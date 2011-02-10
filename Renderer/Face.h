#pragma once

#include <stdlib.h>
#include <math.h>
#include <vector>

using namespace std;

class Face {
	public:
		Face();
		~Face();

		void addVertex(int v, int vt, int vn);

		vector<int> vertices;
		vector<int> texCoords;
		vector<int> normals;
};
