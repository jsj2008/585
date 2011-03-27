#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H
#include <Renderer/GLDebugDrawer.h>
#include "Renderer/RenderObject.h"

class Physics;
class Renderer;
class Sound;
class Models;
class Window;
class Obstacles;
class JeepManager;

class MainController{
public:
	MainController(Window &);
	virtual ~MainController();
	void tick(unsigned long);
	static void restart();
	static void addActor(Actor *);
	static Sound * Audio();
	
protected:
    bool countDown(unsigned long, bool restart=false);
    void tickMenu(unsigned long);
	static MainController * ptr;
	ActorList actorList;
	ActorList obstacleList;
	GLDebugDrawer debugger;
	Renderer * renderer;
	Physics * const physics;
		
	JeepManager * jeepManager;
	Obstacles * obstacles;
	
	Models * models;
	Sound * const audio;
	
	Window & window;
	
    bool counting;
    bool inMenu;
    bool startMenu;
    bool menuSwitch;
    int menuCount;
    
    btVector3 pos;
	
};
#endif
