#pragma once

#include "depend.h"
#include "Shader.h"
#include "Common/Actor.h"
#include "Driving/JeepManager.h"
#include "Driving/JeepActor.h"
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
#include "RenderObject.h"

#define MAX_TEXTURES 4

using namespace std;

class Renderer {
public:
	Renderer(IWindow const &, ActorList const & actorList, JeepManager * jeepManager = NULL);
	~Renderer();
	void step();
	void reset();
	void resetView();
	void setCamera(btVector3 const &, btVector3 const &, btVector3 const & = btVector3(0,1,0) );	//giant hack
	void setMessage(string const & texName);
    void initialize();
	void startGame();
	void showPlace(int);
    void loadingMessage(string const & texName);
    JeepManager * jeepManager;
    
	//GLuint listIndex;
	
protected:
	void paintGL();
	void initializeGL();
    void initializeGL2();
	void resizeGL(int w, int h);
	
private:
	
    static Renderer * ptr;
	
	IWindow * window;
	
	void renderObjects();
	void renderJeeps();
	void drawSky();
	void initSky();
	void drawGround();
	void initGround();
	void drawGroundNormals();
	void applyGroundShader();
	void applyObjectShader();
	void drawAxes();
	void updateCamera();
	void setProjection();
	void setUIProjection();

	void loadGround3DTexture(string filename);
	void loadObject3DTexture(string filename);
	void loadGroundTextures();
	void loadObjectTextures();
	bool loadTexture(string name, GLuint *texID);
	void drawMessage();
	void drawPlayerPlace(int place);
	void groundTexCoord(int x, int z, bool xend, bool zend);

	btVector3 camPos;				// Position of the camera
	btVector3 camLook;				// Point that the camera is looking at
	btVector3 camUp;				// Up vector for the camera TODO: are these in use? Will they be later?

	btVector3 lightPos;				// The current position of the light

	double ratio;					// Aspect ratio of the window
	int width;						// Width of the window
	int height;						// Height of the window

	bool showMessage;
	bool gameStarted;

	ActorList const & actorList;
	Shader* groundShader;
	Shader* objectShader;
	Shader* basicShader;
	int basicShaderTexLocS;				// Uniform location for basic shader texure
	vector<GLuint*> shaderTexturesG;
	vector<GLuint*> shaderTexturesO;
	vector<GLuint*> placeNumbers;

	AttributeData* attrDataG;		// Contains attribute information about the shader
	OptionsData* optDataG;			// Contains optional details about the shader
	TextureData* texDataG;			// Holds information about the textures used by the shader
	AttributeData* attrDataO;		// Contains attribute information about the shader
	OptionsData* optDataO;			// Contains optional details about the shader
	TextureData* texDataO;			// Holds information about the textures used by the shader

	/*Uniform Locations for the ground shader*/
	int xAttrLocG;		// X attribute info from attrData
	int xModLocG;
	int xZminLocG;
	int xFlipLocG;

	int yAttrLocG;		// Y attribute info from attrData
	int yModLocG;
	int yZminLocG;
	int yFlipLocG;

	int zAttrLocG;		// Z attribute info from attrData
	int zModLocG;
	int zZminLocG;
	int zFlipLocG;

	int numTexLocG;		// Number of textures actually in use
	int texPosLocG;		// Positions of the 2D textures in the 3D texture (an array)
	int texHskewLocG;	// Horizontal skews of the 2D textures (an array)
	int texVskewLocG;	// Vertical skews of the 2D textures (an array)

	int tex0LocG;		// Up to four 2D textures to be used in the 3D texture
	int tex1LocG;
	int tex2LocG;
	int tex3LocG;
	int groundTexLocG;	// Texture to apply to the ground
	int cliffTexLocG;	// Texture to apply to slopes
	int groundNormalMapLocG;	// Bump map texture
	int cliffNormalMapLocG;	// Bump map texture
	
	int jeep0ShadowPosLocG; // Locations to draw the jeep shadows in the shader
	int jeep1ShadowPosLocG;
	int jeep2ShadowPosLocG;
	int jeep3ShadowPosLocG;
	int jeep4ShadowPosLocG;
	int jeep5ShadowPosLocG;
	int jeep6ShadowPosLocG;
	int jeep7ShadowPosLocG;
	int jeep8ShadowPosLocG;
	int jeep9ShadowPosLocG;
	int jeep10ShadowPosLocG;

	int tangentLocG;		// Tangent for used in bump mapping

	int autoDiffuseLocG;		// Specifies whether or not to apply automatic diffuse lighting
	int autoSpecularLocG;	// Specifies whether or not to apply automatic specular lighting

	/*Uniform Locations for the object shader*/
	int xAttrLocO;		// X attribute info from attrData
	int xModLocO;
	int xZminLocO;
	int xFlipLocO;

	int yAttrLocO;		// Y attribute info from attrData
	int yModLocO;
	int yZminLocO;
	int yFlipLocO;

	int zAttrLocO;		// Z attribute info from attrData
	int zModLocO;
	int zZminLocO;
	int zFlipLocO;

	int numTexLocO;		// Number of textures actually in use
	int texPosLocO;		// Positions of the 2D textures in the 3D texture (an array)
	int texHskewLocO;	// Horizontal skews of the 2D textures (an array)
	int texVskewLocO;	// Vertical skews of the 2D textures (an array)

	int tex0LocO;		// Up to four 2D textures to be used in the 3D texture
	int tex1LocO;
	int tex2LocO;
	int tex3LocO;
	int colourMapLocO;	// Colourmap to apply in the shader
	int normalMapLocO;	// Bump map texture

	int tangentLocO;		// Tangent for used in bump mapping

	int autoDiffuseLocO;		// Specifies whether or not to apply automatic diffuse lighting
	int autoSpecularLocO;	// Specifies whether or not to apply automatic specular lighting

	// Heightmap rendering
	HeightMap* hm;
	vector<vector<Point> > mapVertexNormals;	// Normals of the heightmap for smooth shading
	vector<vector<Point> > mapVertexTangents;	// Tangents of the heightmap for bump mapping

	float xscale;			// X scale factor for the heightmap
	float yscale;			// Vertical scale factor for the heightmap
	float zscale;			// Z scale factor for the heightmap
	GLuint groundGeometry;	// Display list containing the ground information
	GLuint groundTex;		// The texture applied to the ground
	GLuint cliffTex;		// The texture applied to the slopes
	GLuint groundBump;		// The bump map for the ground
	GLuint cliffBump;		// The bump map for the cliffs

	GLuint messageTex;		// The image for the UI message
	
	RenderObject sky;		// The sky dome model
};