#include "Model.h"

Model::Model(string name) {
	vertices = vector<btVector3*>();
	texCoords = vector<btVector3*>();
	normals = vector<btVector3*>();
	faces = vector<Face>();

	if (!name.empty()) load(name);
}

Model::~Model() { }

// Loads all the vertex, texture coordinate, normal, and face information in the given obj file
void Model::load(string filename) {
	ifstream inFile;
	inFile.open(filename.c_str());

	char line[1000];
	vector<string> tokens;
	vector<string> indices;

	cout << "Loading model " << filename.c_str() << endl;

	while (!inFile.eof()) {
		inFile.getline(line, 1000);
		tokens = splitString(string(line));
		
		if (!tokens.empty()) {
			if (tokens.front().compare("v")==0 && tokens.size() == 4)
				vertices.push_back(new btVector3(atof(tokens.at(1).c_str()), atof(tokens.at(2).c_str()), atof(tokens.at(3).c_str())));
			if (tokens.front().compare("vt")==0 && (tokens.size() == 3 || tokens.size() == 4))
				texCoords.push_back(new btVector3(atof(tokens.at(1).c_str()), atof(tokens.at(2).c_str()), 0));
			if (tokens.front().compare("vn")==0 && tokens.size() == 4)
				normals.push_back(new btVector3(atof(tokens.at(1).c_str()), atof(tokens.at(2).c_str()), atof(tokens.at(3).c_str())));
			if (tokens.front().compare("f")==0 && tokens.size() >= 4)  {
				Face face = Face();
				for (int i = 1; i < tokens.size(); i++) {
					indices = splitString(tokens.at(i), '/', true);
					if (indices.size() == 3) {
						if (indices.at(1).compare("") == 0)		// vertex and normal given, no texture coordinates
							face.addVertex(atoi(indices.at(0).c_str()), 0, atoi(indices.at(2).c_str()));
						else									// vertex, normal and texture coordinate all given
							face.addVertex(atoi(indices.at(0).c_str()), atoi(indices.at(1).c_str()), atoi(indices.at(2).c_str()));
					} else  if (indices.size() == 2) {			// vertex and texture coordinates given, no normal	
						face.addVertex(atoi(indices.at(0).c_str()), atoi(indices.at(1).c_str()), atoi(indices.at(0).c_str())); // set v = vn for calculated normals
					} else {									// only vertex given
						face.addVertex(atoi(indices.at(0).c_str()), 0, atoi(indices.at(0).c_str())); // set v = vn for calculated normals
					}
				}
				faces.push_back(face);
			}
		}
	}

	if (normals.size() == 0) { // If normals are not defined, they need to be calculated
		cout << "Calculating normals for " << filename.c_str() << endl;

		btVector3* normal;
		int count;
		for (int i = 1; i <= vertices.size(); i++) {
			normal = new btVector3(0,0,0);
			count = 0;
			for (int j = 0; j < faces.size(); j++) {
				if (faces.at(j).containsIndex(i)) {
					*normal += getFaceNormal(j);
					count++;
				}
			}
			*normal /= count;
			normals.push_back(normal);
		}
	}

	inFile.close();
}

// Convenient for splitting lines in the obj file as well as face definitions (separated by '/')
vector<string> Model::splitString(string str, char delim, bool leaveBlanks) {
	int start = 0;
	vector<string> result;
	for (int i = 0; i < str.length(); i++) {
		if (str.at(i) == delim) {
			string substr = str.substr(start, i-start);
			if (!substr.empty() || leaveBlanks)
				result.push_back(str.substr(start, i-start));
			start = i+1;
		}
	}
	string substr = str.substr(start, str.length());
	if (!substr.empty()) result.push_back(str.substr(start, str.length()));
	return result;
}

btVector3 Model::getFaceNormal(int index) {
	return ((*vertices.at(faces.at(index).vertices.front()-1))-(*vertices.at(faces.at(index).vertices.at(1)-1))).cross(
			(*vertices.at(faces.at(index).vertices.front()-1))-(*vertices.at(faces.at(index).vertices.back()-1)));
}
