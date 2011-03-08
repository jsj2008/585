#pragma once

#include <fstream>
#include <string>
using namespace std;

enum attributeType {DIFFUSE, SUPERDIFFUSE, SPECULAR, REFRACTION, ORIENTATION, DEPTH,
	LIGHT, CAMERA, REFLECTLONG, REFLECTLAT, MAPLONG, MAPLAT};

class AttributeData {
public:
	
	static const int numAttributes = 12;

	attributeType xAttr;	// The attribute type in the x axis of the 3D texture shader
	float xMod;				// Attribute modifier
	float xZmin;			// Another modifier, it's effect depends on the attribute type (e.g. distance of fog start in Depth)
	bool xFlip;				// Whether or not to flip this axis

	attributeType yAttr;	// The attribute type in the y axis of the 3D texture shader
	float yMod;
	float yZmin;
	bool yFlip;

	attributeType zAttr;	// The attribute type in the z axis of the 3D texture shader
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
