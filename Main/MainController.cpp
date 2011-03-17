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
#include "Renderer/Models.h"
#include "UI/Window.h"

MainController * MainController::ptr = NULL;

void cleanup(){
	MainController::Audio()->KillALData();
}

MainController::MainController(Window const & window) : 
physics(PhysicsFactory::newPhysics(actorList, debugger) ),
audio(new Sound() ),
window(window)
{

	if(ptr == NULL)
		ptr = this;	
	
	models = new Models();
	
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

	JeepActor * human = jeepManager.getHuman();
	human->registerAudio(audio);
	audio->beginLevel();
	audio->playMusic();

}

void MainController::tick(unsigned long interval)
{
	renderer->step();
	physics->step(interval / 1000.0);
	jeepManager.renderTick();
	
	/*giant hack for camera*/

	JeepActor* player = jeepManager.getHuman();
	static btVector3 pos = btVector3(0,0,0);
	btVector3 look = player->pos;
	btVector3 behind = quatRotate(player->orientation, btVector3(-0.8,0.4,0) );
	pos += (look + 30*behind - pos ) / 15.0;
	
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
	
	delete models;
	
	delete physics;
	delete renderer;
}