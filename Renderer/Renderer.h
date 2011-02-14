#ifndef RENDERER_H
#define RENDERER_H

#include "UI/IWindow.h"
#include "Common/Point.h"
#include "Trackball.h"
#include <stdlib.h>
#include <math.h>
#include <LinearMath/btIDebugDraw.h>
#include "depend.h"
#include <Renderer/Camera.h>

using namespace std;

class Renderer {
public:
	//Renderer(const ActorList* actorList = 0, QWidget* parent = 0);
	Renderer(IWindow const &);
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
	void drawQuad(Point tl, Point tr, Point bl, Point br);
	void drawCube(Point tlb, Point trb, Point tlf, Point trf, Point blb, Point brb, Point blf, Point brf);
	void updateMousePosition(int x, int y);
	Point getScreenPosition(int x, int y);
	
	int mouseX;
	int mouseY;

	Point camPos;				// Position of the camera
	Point camLook;				// Point that the camera is looking at
	Vector3 camUp;				// Up vector for the camera

	double ratio;
	int width;
	int height;	

	Trackball trackball;		// The trackball for rotating the surface*/

};
#endif