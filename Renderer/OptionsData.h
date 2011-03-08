#pragma once

#include <fstream>
#include <string>
#include <btBulletDynamicsCommon.h>
using namespace std;

class OptionsData {
public:

	bool autoDiffuse;		// Whether or not the shader should apply automatic diffuse lighting on top of the shader colour
	bool autoSpecular;		// Whether or not the shader should apply automatic specular lighting on top of the shader colour	
	btVector3 bgColor;		// Not currently used by this program
	btVector3 ambientColor;	// Not currently used by this program

	OptionsData();
	~OptionsData();

	void resetValues();
	void load(string filename);
};
