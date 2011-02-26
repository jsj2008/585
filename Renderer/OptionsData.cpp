#include "OptionsData.h"

OptionsData::OptionsData() {
	resetValues();
}

OptionsData::~OptionsData() { }

void OptionsData::resetValues() {
	autoDiffuse = false;
	autoSpecular = false;
	bgColor = btVector3(0.0f, 0.0f, 0.0f);
	ambientColor = bgColor;
}

// Loads the options data saved in a .tx3 file
void OptionsData::load(string filename) {
	resetValues();

	ifstream inFile;
	inFile.open(filename.c_str());
	double x, y, z;

	while (!inFile.eof()) {
		inFile.ignore(1024, '#');
		inFile.unget();
		if (inFile.peek() == '#') {
			string line;
			inFile >> line;

			if (line == "#autodiffuse") {
				autoDiffuse = true;
			} else if (line == "#autospecular") {
				autoSpecular = true;
			} else if (line == "#bgcolor") {
				inFile >> x >> y >> z;
				bgColor = btVector3(x, y, z);
			} else if (line == "#ambientcolor") {
				inFile >> x >> y >> z;
				ambientColor = btVector3(x, y, z);
			}
		}
	}
	inFile.close();
}
