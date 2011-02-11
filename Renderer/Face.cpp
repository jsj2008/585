#include "Face.h"

Face::Face() {
	vertices = vector<int>();
	texCoords = vector<int>();
	normals = vector<int>();
}

Face::~Face() { }

void Face::addVertex(int v, int vt, int vn) {
	vertices.push_back(v);
	texCoords.push_back(vt);
	normals.push_back(vn);
}

bool Face::containsIndex(int v) {
	for (int i = 0; i < vertices.size(); i++)
		if (vertices.at(i) == v) return true;
	return false;
}
