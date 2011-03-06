#include "MainController.h"
#include <Renderer/Renderer.h>
#include <Common/Actor.h>
#include "Renderer/depend.h"
#include "Common/SettingsFactory.h"
#include "Driving/JeepManager.h"
#include "Driving/JeepActor.h"
#include "Physics/PhysicsFactory.h"
#include "Audio/Sound.h"
#include <iostream>

MainController * MainController::ptr = NULL;

void cleanup(){
	MainController::Audio()->KillALData();
}

MainController::MainController() : 
physics(PhysicsFactory::newPhysics(actorList, debugger) ),
audio(new Sound() )
{

	if(ptr == NULL)
		ptr = this;
	
	obstacles.initialize(obstacleList);
	physics->newActors(obstacleList);	//adds the obstacles
	jeepManager.initialize(physics, window.aInput);
	renderer = new Renderer(window, actorList);
	
	/*audio code*/
	alutInit(NULL, 0);
	alGetError();
	if(audio->LoadALData() == AL_FALSE)
		std::cout << "could not load audio" << std::endl;

	atexit(cleanup);

	audio->SetListenerValues();
	audio->playMusic();
	// audio->increasePitch(0.1);

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

void MainController::restart()
{
	ptr->jeepManager.restart();
}

Sound * MainController::Audio()
{
	return ptr->audio;
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