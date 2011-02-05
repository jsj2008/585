#pragma once

#include <fstream>
#include <string>
using namespace std;

enum attributeType {DIFFUSE, SUPERDIFFUSE, SPECULAR, REFRACTION, ORIENTATION, DEPTH,
	LIGHT, CAMERA, REFLECTLONG, REFLECTLAT, MAPLONG, MAPLAT};

class AttributeData {
public:
	
	static const int numAttributes = 12;

	attributeType xAttr;
	float xMod;
	float xZmin;
	bool xFlip;

	attributeType yAttr;
	float yMod;
	float yZmin;
	bool yFlip;

	attributeType zAttr;
	float zMod;
	float zZmin;
	bool zFlip;

	AttributeData();
	~AttributeData();

	void resetValues();
	static string getName(attributeType type);
	void load(string filename);

private:

	static string fileName(attributeType type);
	static attributeType parseAttribute(string name);

};
