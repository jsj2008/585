#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H
#include <Renderer/GLDebugDrawer.h>
#include "Renderer/RenderObject.h"
#include "Driving/JeepManager.h"
#include "Obstacles.h"

class Physics;
class Renderer;
class Sound;
class Models;
class Window;


class MainController{
public:
	MainController(Window const &);
	virtual ~MainController();
	void tick(unsigned long);
	static void restart();
	static void addActor(Actor *);
	static Sound * Audio();
	
protected:
	static MainController * ptr;
	ActorList actorList;
	ActorList obstacleList;
	GLDebugDrawer debugger;
	Renderer * renderer;
	Physics * const physics;
		
	JeepManager jeepManager;
	Obstacles obstacles;
	
	Models * models;
	Sound * const audio;
	
	Window const & window;
	
};
#endif
