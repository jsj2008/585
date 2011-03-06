#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H
#include "UI/Window.h"
#include <Renderer/GLDebugDrawer.h>
#include "Renderer/RenderObject.h"
#include "Driving/JeepManager.h"
#include "Obstacles.h"

class Physics;
class Renderer;



class MainController{
public:
	MainController();
	virtual ~MainController();
	void tick(unsigned long);
	static void restart();
	static void addActor(Actor *);
	Window window;
	
protected:
	static MainController * ptr;
	ActorList actorList;
	ActorList obstacleList;
	GLDebugDrawer debugger;
	Renderer * renderer;
	Physics * const physics;
		
	JeepManager jeepManager;
	Obstacles obstacles;
	
};
#endif
