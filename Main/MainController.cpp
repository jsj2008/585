#include "MainController.h"
#include <Renderer/Renderer.h>
#include <Common/Actor.h>
#include "Renderer/depend.h"
#include "Common/SettingsFactory.h"
#include "Driving/JeepManager.h"
#include "Driving/JeepActor.h"
#include "Physics/PhysicsFactory.h"

MainController * MainController::ptr = NULL;

MainController::MainController() : 
physics(PhysicsFactory::newPhysics(actorList, debugger) )
{

	if(ptr == NULL)
		ptr = this;
	
	obstacles.initialize(obstacleList);
	physics->newActors(obstacleList);	//adds the obstacles
	jeepManager.initialize(physics, window.aInput);
	renderer = new Renderer(window, actorList);

}

void MainController::tick(unsigned long interval)
{
	renderer->step();
	physics->step(interval / 1000.0);
	jeepManager.renderTick();
	
	/*giant hack for camera*/

	JeepActor* player = jeepManager.getHuman();
	static btVector3 pos = btVector3(9,11,15);
	btVector3 look = player->pos;
	btVector3 behind = quatRotate(player->orientation, btVector3(-1,0.4,0) );
	pos += (look + 30*behind - pos ) / 30.0;
	
	renderer->setCamera(pos,look);
	
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