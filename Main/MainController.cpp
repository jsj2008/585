#include "MainController.h"
#include "Physics/PhysicsFactory.h"
#include <iostream>


MainController::MainController() : 
physics(PhysicsFactory::newPhysics(actorList, debugger) )
{
	SettingsFactory::loadSettings("config/example1.xml");
			
	/*setup various lists*/	
	ActorList temp;
	for(int i=0; i<0; i++)
	{
		Actor * act = new Actor(mCube, Point(0,-3, 0) );
		actorList.push_back(act);
		temp.push_back(act);
	}
	
	Actor * act = new Actor(mPlane, Point(0,-7.5,0));
	actorList.push_back(act);
	temp.push_back(act);
		
	/*pass jeep into physics/renderer but don't add to dynamicWorld (this is done by jeep internally)*/
	jeep = new JeepActor(mChasis, physics );
	actorList.push_back(jeep);
		
	/*setup subcomponents*/
	renderer = new Renderer(window);
	physics->newActors(temp);
	window.run(this);	//launch window

}

void MainController::yield()
{
	
}

void MainController::tick(unsigned long interval)
{
	
	//physics->step( interval / 1000.0 );
	//jeep->tick(interval / 1000.0);
	physics->step(0.016);
	jeep->tick(0.016);
	
}

void MainController::explode()
{
	static int counter = 0;
	ActorList temp;
	
	Real rad = 3.1415926 / 6 * counter++;	//pick random angle
	
	Actor * act = new Actor(mCube, Point(0,-3, 0), Point(cos(rad)*3, 5, sin(rad)*3 ) );
	actorList.push_back(act);
	temp.push_back(act);
		
	physics->newActors(temp);
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