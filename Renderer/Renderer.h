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

#define MAX_TEXTURES 4

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

	void load3DTexture(string filename);
	void loadTextures();
	bool loadTexture(string name, GLuint *texID);

	void groundTexCoord(int x, int z, bool xend, bool zend);

	btVector3 camPos;				// Position of the camera
	btVector3 camLook;				// Point that the camera is looking at
	btVector3 camUp;				// Up vector for the camera TODO: are these in use? Will they be later?

	btVector3 lightPos;				// The current position of the light

	double ratio;					// Aspect ratio of the window
	int width;						// Width of the window
	int height;						// Height of the window

	ActorList const & actorList;
	Shader* shader;
	vector<GLuint*> shaderTextures;

	AttributeData* attrData;		// Contains attribute information about the shader
	OptionsData* optData;			// Contains optional details about the shader
	TextureData* texData;			// Holds information about the textures used by the shader

	/*Uniform Locations for the shader*/
	int xAttrLoc;		// X attribute info from attrData
	int xModLoc;
	int xZminLoc;
	int xFlipLoc;

	int yAttrLoc;		// Y attribute info from attrData
	int yModLoc;
	int yZminLoc;
	int yFlipLoc;

	int zAttrLoc;		// Z attribute info from attrData
	int zModLoc;
	int zZminLoc;
	int zFlipLoc;

	int numTexLoc;		// Number of textures actually in use
	int texPosLoc;		// Positions of the 2D textures in the 3D texture (an array)
	int texHskewLoc;	// Horizontal skews of the 2D textures (an array)
	int texVskewLoc;	// Vertical skews of the 2D textures (an array)

	int tex0Loc;		// Up to four 2D textures to be used in the 3D texture
	int tex1Loc;
	int tex2Loc;
	int tex3Loc;
	int colourMapLoc;	// Colourmap to apply in the shader
	int normalMapLoc;	// Bump map texture

	int tangentLoc;		// Tangent for used in bump mapping

	int autoDiffuseLoc;		// Specifies whether or not to apply automatic diffuse lighting
	int autoSpecularLoc;	// Specifies whether or not to apply automatic specular lighting

	// Heightmap rendering
	HeightMap* hm;
	vector<vector<Point> > mapVertexNormals;	// Normals of the heightmap for smooth shading
	vector<vector<Point> > mapVertexTangents;	// Tangents of the heightmap for bump mapping

	float xscale;			// X scale factor for the heightmap
	float yscale;			// Vertical scale factor for the heightmap
	float zscale;			// Z scale factor for the heightmap
	GLuint groundGeometry;	// Display list containing the ground information
	GLuint groundTex;		// The texture applied to the ground
	GLuint groundBump;		// The bump map for the ground
};