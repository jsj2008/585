#pragma once

#include <fstream>
#include <string>
#include <btBulletDynamicsCommon.h>
using namespace std;

class OptionsData {
public:

	bool autoDiffuse;
	bool autoSpecular;
	btVector3 bgColor;
	btVector3 ambientColor;

	OptionsData();
	~OptionsData();

	void resetValues();
	void load(string filename);
};
