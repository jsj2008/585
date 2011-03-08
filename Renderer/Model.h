#pragma once

#include "Face.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Model {
	public:
		Model(string filename = "");
		~Model();

		void load(string filename);

		vector<btVector3*> vertices;
		vector<btVector3*> texCoords;
		vector<btVector3*> normals;
		vector<Face> faces;				// List of faces in the model, with references to the above vertices, texture coordinates and normals

	private:
		vector<string> splitString(string str, char delim = ' ', bool leaveBlanks = false);
		btVector3 getFaceNormal(int index);
};
