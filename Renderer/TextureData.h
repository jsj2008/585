#pragma once

#include <string>
#include <vector>
#include <list>
#include "Texture.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <direct.h>
#include <stdlib.h>
using namespace std;

#define MIN_TEXTURE_DISTANCE 0.005f

class TextureData {
public:
	TextureData(int maxTextures);
	~TextureData(void);

	void deleteTexture(int index);
	int getNumTextures();
	bool textureExists(int index);
	string getTextureFilename(int index);
	float getTexturePos(int index);
	float getTextureVertexPos(int index, int vertex);
	float getTextureHSkew(int index);
	float getTextureVSkew(int index);
	bool getTextureInterpolate(int index);
	vector<int> getTextureIndices();

	void clearAll();
	void load(string filename);
	int getMaxTextures();

private:
	
	int maxTextures;
	int numTextures;
	vector<Texture*> texture;

	bool validTexturePos(int index, float pos, float h_skew, float v_skew);
	bool intersects(int index, float pos, float h_skew, float v_skew, int index2);

	int getNextValidIndex();
	void addTexture(string filename, float pos, float h_skew, float v_skew, bool interpolate);
	string extractDir(string str);
};
