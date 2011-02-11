#include "TextureData.h"

TextureData::TextureData(int maxTextures) {
	texture.resize(maxTextures);
	numTextures = 0;
	this->maxTextures = maxTextures;
}

TextureData::~TextureData() { }

bool TextureData::validTexturePos(int index, float pos, float h_skew, float v_skew) {

	if (pos < 0.0f || pos > 1.0f) return false;
	for (int i = 0; i < 4; i++) {
		float v_pos = Texture::vertexPos(i, pos, h_skew, v_skew);
		if (v_pos < 0.0f || v_pos > 1.0f)
			return false;
	}
	for (int i = 0; i < maxTextures; i++) {
		if (textureExists(i) && i != index) {
			if (intersects(index, pos, h_skew, v_skew, i)) return false;
		}
	}
	return true;
}

bool TextureData::intersects(int index, float pos, float h_skew, float v_skew, int index2) {
	if (index == index2) return false;
	bool before = (pos < texture[index2]->pos);
	for (int i = 0; i < 4; i++) {
		if (before) {
			if (Texture::vertexPos(i, pos, h_skew, v_skew) > texture[index2]->vertexPos(i)-MIN_TEXTURE_DISTANCE)
				return true;
		} else {
			if (Texture::vertexPos(i, pos, h_skew, v_skew) < texture[index2]->vertexPos(i)+MIN_TEXTURE_DISTANCE)
				return true;
		}
	}
	return false;
}

void TextureData::deleteTexture(int index) {
	if (!textureExists(index)) return;

	delete(texture[index]);
	texture[index] = NULL;
	numTextures--;
}

bool TextureData::textureExists(int index) {
	if (index < 0||index >= maxTextures||texture[index] == NULL) return false;
	return true;
}

int TextureData::getNumTextures() {
	return numTextures;
}

string TextureData::getTextureFilename(int index) {
	if (!textureExists(index))
		return "";
	return texture[index]->filename;
}

float TextureData::getTexturePos(int index) {
	if (!textureExists(index))
		return -1.0f;
	return texture[index]->pos;
}

float TextureData::getTextureVertexPos(int index, int vertex) {
	if (!textureExists(index))
		return -1.0f;
	return texture[index]->vertexPos(vertex);
}

float TextureData::getTextureHSkew(int index) {
	if (!textureExists(index))
		return 0.0f;
	return texture[index]->h_skew;
}

float TextureData::getTextureVSkew(int index) {
	if (!textureExists(index))
		return 0.0f;
	return texture[index]->v_skew;
}

bool TextureData::getTextureInterpolate(int index) {
	if (!textureExists(index))
		return false;
	return texture[index]->interpolate;
}

vector<int> TextureData::getTextureIndices() {
	vector<int> index;

	for (int i = 0; i < maxTextures; i++) {
		if (textureExists(i))
			index.push_back(i);
	}
	return index;
}

void TextureData::clearAll() {
	for (int i = 0; i < maxTextures; i++) {
		if (textureExists(i)) deleteTexture(i);
	}
}

void TextureData::load(string filename) {
	clearAll();

	ifstream inFile;
	inFile.open(filename.c_str());

	string savedir = extractDir(filename);

	while (!inFile.eof()) {
		inFile.ignore(1024, '#');
		inFile.unget();
		if (inFile.peek() == '#') {
			string line;
			inFile >> line;
			if (line == "#tex" && numTextures < maxTextures) {
				string texfile;
				float pos;
				float hSkew;
				float vSkew;
				short interpolate;

				inFile >> texfile;
				inFile >> pos;
				inFile >> hSkew;
				inFile >> vSkew;
				inFile >> interpolate;

				texfile = texfile.substr(1);

				/*if (texfile.find(':') == string.npos) {
					string tempdir = savedir;
					texfile = tempdir.append(texfile);
				}*/

				if (validTexturePos(-1, pos, hSkew, vSkew)) {
					addTexture(texfile, pos, hSkew, vSkew, (interpolate == 1));
				}
			}
		}
	}
	inFile.close();
}

int TextureData::getNextValidIndex() {
	for (int i = 0; i < maxTextures; i++) {
		if (texture[i] == NULL)
			return i;
	}
	return -1;
}

void TextureData::addTexture(string filename, float pos, float h_skew, float v_skew, bool interpolate) {
	int new_index = getNextValidIndex();

	texture[new_index] = new Texture();
	texture[new_index]->pos = pos;
	texture[new_index]->h_skew = h_skew;
	texture[new_index]->v_skew = v_skew;
	texture[new_index]->interpolate = interpolate;
	texture[new_index]->filename = filename;

	numTextures++;
}

int TextureData::getMaxTextures() {
	return maxTextures;
}

string TextureData::extractDir(string str) {
	return (str.substr(0, str.find_last_of('/')));
}