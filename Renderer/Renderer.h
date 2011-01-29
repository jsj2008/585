#ifndef RENDERER_H
#define RENDERER_H

#include "Common/Actor.h"
#include "UI/IWindow.h"
#include <stdlib.h>
#include <math.h>
#include <LinearMath/btIDebugDraw.h>
#include "depend.h"

using namespace std;

class Renderer {
public:
	Renderer(IWindow const &, const ActorList* actorList = 0);
	~Renderer();
	void step();
	void reset();
	void resetView();
	//GLuint listIndex;
	
	
protected:
	void paintGL();
	void initializeGL();
	void resizeGL(int w, int h);
	
private:
	
	IWindow * window;
	
	void renderObjects();
	void drawAxes();
	void updateCamera();
	void setProjection();
	void drawQuad(btVector3* tl, btVector3* tr, btVector3* bl, btVector3* br);
	void drawCube(btVector3* tlb, btVector3* trb, btVector3* tlf, btVector3* trf, btVector3* blb, btVector3* brb, btVector3* blf, btVector3* brf);
	void updateMousePosition(int x, int y);
	btVector3* getScreenPosition(int x, int y);
	
	int mouseX;
	int mouseY;

	btVector3 camPos;				// Position of the camera
	btVector3 camLook;				// Point that the camera is looking at
	btVector3 camUp;				// Up vector for the camera

	double ratio;
	int width;
	int height;	

	ActorList* actorList;

};
#endif