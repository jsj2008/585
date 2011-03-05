#include "MainController.h"
#include "Physics/PhysicsFactory.h"
#include <iostream>
#include "Common/SettingsFactory.h"

#include <irrKlang.h>
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
using namespace irrklang;

MainController * MainController::ptr = NULL;


MainController::MainController() : 
physics(PhysicsFactory::newPhysics(actorList, debugger) )
{
	if(ptr == NULL)
		ptr = this;


		//renderTest = RenderObject("testBox.bmp", "ducky.obj");

	 // Actor * act = new Actor(mPlane, planeModel, btVector3(0,-5,0));
	 // actorList.push_back(act);	

	/***********Sound test********/ 
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine) {
		std::cout << "Could not startup sound engine\n" << std::endl;
	} else {
		engine->play2D("sound/TribalGroove.mp3", true);
		engine->play2D("sound/ExoticBirds.mp3", true);
		//engine->play3D("sound/FX/test.wav", vec3df(0,0,2));
	}
	/***********Sound test********/ 
	

	jeepManager = new JeepManager(physics, &actorList, window.aInput);
		
	/*setup subcomponents*/
	renderer = new Renderer(window, actorList);

	window.run(this);	//launch window

}

void MainController::yield()
{
	
}

void MainController::tick(unsigned long interval)
{
	//std::cout << interval << std::endl;
	renderer->step();
	physics->step(interval / 1000.0);
	jeepManager->renderTick();
	
	/*giant hack for camera*/

	JeepActor* player = jeepManager->getHuman();
	static btVector3 pos = btVector3(9,11,15);
	btVector3 look = player->pos;
	btVector3 behind = quatRotate(player->orientation, btVector3(-1,0.4,0) );
	pos += (look + 30*behind - pos ) / 30.0;
	
	renderer->setCamera(pos,look);
	
}

void MainController::explode()
{
/*	static int counter = 0;
	ActorList temp;
	Real rad = 3.1415926 / 6 * counter++;	//pick random angle
	
	Actor * act = new Actor(mCube, cubeModel, btVector3(0,-3, 0), btVector3(cos(rad)*5, 10, sin(rad)*5 ) );
	actorList.push_back(act);
	temp.push_back(act);
		
	physics->newActors(temp);*/
}

void MainController::addActor(Actor * actor)
{
	ptr->actorList.push_back(actor);
}

MainController::~MainController()
{
	for(ActorList::iterator itr = actorList.begin(); itr != actorList.end(); ++itr)
	{
		delete (*itr);
	}
	
	delete physics;
	delete renderer;
}