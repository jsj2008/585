#include "MainController.h"
#include "Physics/PhysicsFactory.h"
#include <iostream>
#include "Common/SettingsFactory.h"


MainController::MainController() : 
physics(PhysicsFactory::newPhysics(actorList, debugger) )
{

	float const & planeY = LoadFloat("config/start.xml", "planeY");
	float const & jeepX = LoadFloat("config/start.xml", "jeepX");
	float const & jeepY = LoadFloat("config/start.xml", "jeepY");
	float const & jeepZ = LoadFloat("config/start.xml", "jeepZ");

	
	renderTest = RenderObject("testBox.bmp", "jeep_final.obj");
	// renderTest = RenderObject("testBox.bmp", "quad.obj");
	//renderTest = RenderObject("testBox.bmp", "ducky.obj");

	/*setup various lists*/	
	ActorList temp;
	for(int i=0; i<1; i++)
	{
		Actor * act = new Actor(mCube, renderTest, btVector3(0,-3, 0) );
		actorList.push_back(act);
		temp.push_back(act);
	}
	
//	Actor * act = new Actor(mPlane, btVector3(0,planeY,0));
	Actor * act = new Actor(mPlane, renderTest, btVector3(0,-5,0));

	actorList.push_back(act);
	temp.push_back(act);
		
	/*pass jeep into physics/renderer but don't add to dynamicWorld (this is done by jeep internally)*/
	jeep = new JeepActor(mChasis, renderTest, physics, window.aInput , btVector3(jeepX, jeepY, jeepZ));
	actorList.push_back(jeep);
		
	/*setup subcomponents*/
	physics->newActors(temp);
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
	physics->step( interval / 1000.0);
	jeep->tick(interval / 1000.0);
	
}

void MainController::explode()
{
	static int counter = 0;
	ActorList temp;
	
	Real rad = 3.1415926 / 6 * counter++;	//pick random angle
	
	Actor * act = new Actor(mCube, renderTest, btVector3(0,-3, 0), btVector3(cos(rad)*5, 10, sin(rad)*5 ) );
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