#pragma once

#include "depend.h"
#include "Shader.h"
#include "Common/Actor.h"
#include "UI/IWindow.h"
#include "AttributeData.h"
#include "OptionsData.h"
#include "TextureData.h"
#include <stdlib.h>
#include <math.h>
#include <LinearMath/btIDebugDraw.h>
#include "Common/SettingsFactory.h"
#include "Physics/Heightmap.h"
#include "Common/Point.h" //I don't want this here but btVector3 is not cooperating

#define MAX_TEXTURES 3

using namespace std;

class Renderer {
public:
	Renderer(IWindow const &, ActorList const & actorList);
	~Renderer();
	void step();
	void reset();
	void resetView();
	void setCamera(btVector3 const &, btVector3 const &);	//giant hack
	//GLuint listIndex;
	
protected:
	void paintGL();
	void initializeGL();
	void resizeGL(int w, int h);
	
private:
	
	IWindow * window;
	
	void renderObjects();
	void drawGround();
	void initGround();
	void drawGroundNormals();
	void applyShader();
	void drawAxes();
	void updateCamera();
	void setProjection();
	btVector3* getScreenPosition(int x, int y);

	void load3DTexture(string filename);
	void loadTextures();
	bool loadTexture(string name, GLuint *texID);

	void groundTexCoord(int x, int z, bool xend, bool zend);

	btVector3 camPos;				// Position of the camera
	btVector3 camLook;				// Point that the camera is looking at
	btVector3 camUp;				// Up vector for the camera

	btVector3 lightPos;

	double ratio;
	int width;
	int height;	

	ActorList const & actorList;
	Shader* shader;
	vector<GLuint*> shaderTextures;

	AttributeData* attrData;
	OptionsData* optData;
	TextureData* texData;

	/*Uniform Locations*/
	int xAttrLoc;
	int xModLoc;
	int xZminLoc;
	int xFlipLoc;

	int yAttrLoc;
	int yModLoc;
	int yZminLoc;
	int yFlipLoc;

	int zAttrLoc;
	int zModLoc;
	int zZminLoc;
	int zFlipLoc;

	int numTexLoc;
	int texPosLoc;
	int texHskewLoc;
	int texVskewLoc;
	int texInterpLoc;

	int tex0Loc;
	int tex1Loc;
	int tex2Loc;
	int tex3Loc;

	int autoDiffuseLoc;
	int autoSpecularLoc;

	// Heightmap rendering
	HeightMap* hm;
	vector<vector<Point>> mapVertexNormals;
	float xscale;
	float yscale;
	float zscale;
	GLuint groundGeometry;
	GLuint groundTex;
};