#include "AttributeData.h"

AttributeData::AttributeData() {
	resetValues();
}

AttributeData::~AttributeData() { }

void AttributeData::resetValues() {
	xAttr = DIFFUSE;
	xMod = 1.0f;
	xZmin = 0.5f;
	xFlip = false;

	yAttr = SPECULAR;
	yMod = 1.0f;
	yZmin = 0.5f;
	yFlip = false;

	zAttr = ORIENTATION;
	zMod = 1.0f;
	zZmin = 0.5f;
	zFlip = false;
}

string AttributeData::getName(attributeType type) {
	switch (type) {
		case DIFFUSE: return "Diffuse";break;
		case SUPERDIFFUSE: return "Super Diffuse";break;
		case SPECULAR: return "Specular";break;
		case REFRACTION: return "Refraction";break;
		case ORIENTATION: return "Orientation";break;
		case DEPTH: return "Depth";break;
		case LIGHT: return "Light";break;
		case CAMERA: return "Camera";break;
		case REFLECTLONG: return "Reflection Longitude";break;
		case REFLECTLAT: return "Reflection Latitude";break;
		case MAPLONG: return "Map Longitude";break;
		case MAPLAT: return "Map Latitude";break;
	}
	return "Diffuse";
}

string AttributeData::fileName(attributeType type) {
	switch (type) {
		case SUPERDIFFUSE: return "SuperDiffuse";break;
		case REFLECTLONG: return "ReflectionLongitude";break;
		case REFLECTLAT: return "ReflectionLatitude";break;
		case MAPLONG: return "MapLongitude";break;
		case MAPLAT: return "MapLatitude";break;
	}

	return getName(type);
}

attributeType AttributeData::parseAttribute(string name) {
	for (attributeType a = DIFFUSE; (int) a < numAttributes; a = (attributeType) (((int) a)+1)) {
		if (name == fileName(a))
			return a;
	}
	return DIFFUSE;
}

void AttributeData::load(string filename) {

	ifstream inFile;
	inFile.open(filename.c_str());

	while (!inFile.eof()) {
		inFile.ignore(1024, '#');
		inFile.unget();
		if (inFile.peek() == '#') {
			string line;
			inFile >> line;
			if (line == "#att") {
				string attname;

				short flipNum;

				inFile >> attname;
				xAttr = parseAttribute(attname);
				inFile >> xMod;
				inFile >> xZmin;
				inFile >> flipNum;
				xFlip = (bool) flipNum;

				inFile >> attname;
				yAttr = parseAttribute(attname);
				inFile >> yMod;
				inFile >> yZmin;
				inFile >> flipNum;
				yFlip = (bool) flipNum;

				inFile >> attname;
				zAttr = parseAttribute(attname);
				inFile >> zMod;
				inFile >> zZmin;
				inFile >> flipNum;
				zFlip = (bool) flipNum;

				break;
			}
		}
	}
	inFile.close();
}
