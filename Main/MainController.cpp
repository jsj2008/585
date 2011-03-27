#include "Common/Debug.h"
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
#include "UI/Input.h"


MainController * MainController::ptr = NULL;

void cleanup(){
	MainController::Audio()->KillALData();
}

MainController::MainController(Window & window) : 
physics(PhysicsFactory::newPhysics(actorList, debugger) ),
audio(new Sound() ),
window(window),
counting(true),
inMenu(true),
startMenu(false),
menuSwitch(true),
menuCount(0)
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
    // audio->beginLevel();
    // audio->playMusic();

}

void MainController::tickMenu(unsigned long interval)
{
    renderer->step();   //ensures a nice background
    jeepManager->renderTick();  //keep this in case it's paused
    
    if(!startMenu)
    {
        static std::string menus[] = {"data/UI/paused_0.png", "data/UI/paused_1.png", "data/UI/paused_2.png", "data/UI/paused_3.png"};
        
        if(menuCount < 0)   //all the way up
        {
            menuCount = 0;
            menuSwitch = false; //don't bother reloading
        }else if(menuCount > 3)   //all the way down
        {
            menuCount = 3;  //for now only 0 menus
            menuSwitch = false; //don't bother reloading
        }
        
        if(menuSwitch)
        {
            renderer->setMessage(menus[menuCount]);
            menuSwitch = false;
        }
        
        JeepActor* player = jeepManager->getHuman();
    	btVector3 look = player->pos;
        static btScalar angle = 0.0;
        angle += 0.005;
        btVector3 behind = quatRotate(btQuaternion(btVector3(0,1,0), angle) * player->orientation,  btVector3(-0.8,0.4, 0) );
    	pos += (look+ 25*behind - pos ) / 10.0;

    	renderer->setCamera(pos,look);
    	    	
    }else   //proper menu
    {
        
    }
    
	//check user input
	static int keyCount = 0;    //big hack because our controller input is awesome
    keyCount += interval;
    if(keyCount < 50)
        return;    
	
	if(window.aInput->AcceleratePressed)  //go up
	{
        keyCount = 0;
        menuCount --;
        menuSwitch = true;
	}
	
	if(window.aInput->BrakePressed)  //go down
	{
        keyCount = 0;
        menuCount ++;
        menuSwitch = true;
	}
	
	if(window.aInput->EBrakePressed)    //stand in for enter
	{
        renderer->setMessage("");
        inMenu = false;
	}
	        LOG("menuCount " << menuCount, "temp");
	
	
    
    
}

void MainController::tick(unsigned long interval)
{
    if(interval > 100)  //huge hack but seems to work
    {
        std::cout << "LAG" << std::endl;
        return;
    }
    
    if(inMenu)
    {
        tickMenu(interval); //if we're in menu mode then don't do these other steps
        return;
    }
    
    
    if(counting)
    {
        counting = countDown(interval);
    }else
    {
        
    }
        
	renderer->step();
	physics->step(interval / 1000.0);
	jeepManager->renderTick();
	
	/*giant hack for camera*/

	JeepActor* player = jeepManager->getHuman();
	btVector3 look = player->pos;
	btVector3 behind = quatRotate(player->orientation, btVector3(-0.8,0.4, 0) );
	pos += (look+ 25*behind - pos ) / 10.0;
	
	renderer->setCamera(pos,look);
	
	//check for menu input
	if(window.aInput->Escape)    //hack for start menu or something
	{
        inMenu = true;
        menuSwitch = true;
        startMenu = false;
        menuCount = 0;
	}
	
}


bool MainController::countDown(unsigned long interval, bool restart)
{
    static std::string imgs[] = {"", "data/UI/0.png", "data/UI/1.png", "data/UI/2.png", "data/UI/3.png"};
    static int timer = 0;
    static int count = 4;
    
    if(restart)
    {
        timer = 0;
        count = 4;
    }
    
    timer += interval;
    if(timer > 1000)
    {
        std::cout << "count" << std::endl;
        renderer->setMessage(imgs[count]);
        count -= 1;
        timer = 0;
    }
    
    if(count == 0)  //GO
    {
        jeepManager->release();
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
    ptr->counting = true;
    ptr->countDown(0, true);    //restart countdown
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
}