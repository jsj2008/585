#include "MainController.h"
#include "Physics/PhysicsFactory.h"
#include <iostream>
#include "Common/SettingsFactory.h"

// #include <irrKlang.h>
// #pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
// using namespace irrklang;

MainController * MainController::ptr = NULL;


MainController::MainController() : 
physics(PhysicsFactory::newPhysics(actorList, debugger) ),
rockModel("ground_wrap.bmp", "", "models/jeep2_flipx.obj", 10)
{
	if(ptr == NULL)
		ptr = this;
	
	Actor * c = new Actor(mCube, rockModel, btVector3(0, 0, 10));
	obstacles.push_back(c);

	jeepManager = new JeepManager(physics, &actorList, window.aInput);

	actorList.push_back(c);	
	physics->newActors(obstacles);
	/*setup subcomponents*/
	renderer = new Renderer(window, actorList);

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