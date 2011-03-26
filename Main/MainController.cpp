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
#include "Obstacles.h"


MainController * MainController::ptr = NULL;

void cleanup(){
	MainController::Audio()->KillALData();
}

MainController::MainController(Window & window) : 
physics(PhysicsFactory::newPhysics(actorList, debugger) ),
audio(new Sound() ),
window(window)
{

	if(ptr == NULL)
		ptr = this;	
		
	renderer = new Renderer(window, actorList);
	window.renderer = renderer;
    window.loadScreen();
	
    renderer->initialize();
	models = new Models();
    obstacles = new Obstacles();
	obstacles->initialize(obstacleList);
	physics->newActors(obstacleList);	//adds the obstacles
    jeepManager = new JeepManager();
	jeepManager->initialize(physics, window.aInput);
    renderer->jeepManager = jeepManager; //update the pointer
    
	/*audio code*/
	alutInit(NULL, 0);
	alGetError();
	if(audio->LoadALData() == AL_FALSE)
		std::cout << "could not load audio" << std::endl;

	atexit(cleanup);

	JeepActor * human = jeepManager->getHuman();
	human->registerAudio(audio);
	audio->beginLevel();
	audio->playMusic();

}

void MainController::tick(unsigned long interval)
{
    if(interval > 100)  //huge hack but seems to work
    {
        std::cout << "LAG" << std::endl;
        return;
    }
    
    static bool counting = true;
    if(counting)
    {
        counting = countDown(interval);
    }
        
	renderer->step();
	physics->step(interval / 1000.0);
	jeepManager->renderTick();
	
	/*giant hack for camera*/

	JeepActor* player = jeepManager->getHuman();
	static btVector3 pos = btVector3(0,0,0);
	btVector3 look = player->pos;
	btVector3 behind = quatRotate(player->orientation, btVector3(-0.8,0.4, 0) );
	pos += (look+ 25*behind - pos ) / 10.0;
	
	renderer->setCamera(pos,look);
	
}

bool MainController::countDown(unsigned long interval)
{
    static std::string imgs[] = {"", "data/UI/0.png", "data/UI/1.png", "data/UI/2.png", "data/UI/3.png"};
    static int timer = 0;
    static int count = 4;
    timer += interval;
    if(timer > 1000)
    {
        std::cout << "count" << std::endl;
        renderer->setMessage(imgs[count]);
        count -= 1;
        timer = 0;
    }
    
    if(count < 0)
        return false;
    return true;
        
}

void MainController::addActor(Actor * actor)
{
	ptr->actorList.push_back(actor);
}

void MainController::restart()
{
	ptr->jeepManager->restart();
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
    delete jeepManager;
    delete obstacles;
}