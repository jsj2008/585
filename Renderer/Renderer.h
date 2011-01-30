#ifndef RENDERER_H
#define RENDERER_H

#include "depend.h"
#include "Shader.h"
#include "Common/Actor.h"
#include "UI/IWindow.h"
#include <stdlib.h>
#include <math.h>
#include <LinearMath/btIDebugDraw.h>

using namespace std;

class Renderer {
public:
	Renderer(IWindow const &, ActorList const & actorList);
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
	void drawQuad(btVector3 const & tl, btVector3 const & tr, btVector3 const & bl, btVector3 const & br);
	void drawCube(btVector3 const & tlb, btVector3 const & trb, btVector3 const & tlf, btVector3 const & trf, btVector3 const & blb, btVector3 const & brb, btVector3 const & blf, btVector3 const & brf);
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

	ActorList const & actorList;
	Shader* shader;

};
#endif