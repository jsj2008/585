#pragma once

#include <QGLWidget>
#include "../Common/Point.h"
#include "Trackball.h"
#include <QWidget>
#include <stdlib.h>
#include <math.h>
#include <LinearMath/btIDebugDraw.h>

using namespace std;

class Renderer;

class Renderer : public QGLWidget, public btIDebugDraw {
	Q_OBJECT
public:
	//Renderer(const ActorList* actorList = 0, QWidget* parent = 0);
	Renderer(QWidget* parent = 0);
	~Renderer();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	void reset();
	void resetView();
	
	
	/*Bullet debugger*/
	virtual void drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color);
	virtual void drawContactPoint (const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color){}
	virtual void reportErrorWarning (const char *warningString){}
	virtual void draw3dText (const btVector3 &location, const char *textString){}
	virtual void setDebugMode (int debugMode){}
	virtual int getDebugMode () const {}

signals:

public slots:

protected:
	void paintGL();
	void initializeGL();
	void resizeGL(int w, int h);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

private:
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
