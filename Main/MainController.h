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
class JeepActor;

class MainController{
public:
	MainController(Window &);
	virtual ~MainController();
	void tick(unsigned long);
	static void restartJeep(JeepActor *);
	static void restart();
	static void addActor(Actor *);
	static void finishGame( std::vector<int> &);
	
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
	Sound * audio;
	
	Window & window;
	
    bool counting;
    bool inMenu;
    bool startMenu;
    bool menuSwitch;
    int menuCount;
	bool finished;
	bool inCar;
	bool wasOut;
	int place;
    
    btVector3 pos;
    
    //audio files
    unsigned int gameMusic;
    unsigned int menuMusic;
	unsigned int readySource;
	
};
#endif
