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

MainController::MainController(Window & window) : 
physics(PhysicsFactory::newPhysics(actorList, debugger) ),
window(window),
counting(true),
inMenu(true),
startMenu(true),
menuSwitch(true),
menuCount(0),
pos(btVector3(0,0,0))
{

	if(ptr == NULL)
		ptr = this;	
		
	renderer = new Renderer(window, actorList);
	window.renderer = renderer;
    window.loadScreen();

    renderer->initialize();
    
    audio = new Sound();
    gameMusic = audio->addSource("data/audio/TribalGroove.wav", true);  //music
    menuMusic = audio->addSource("data/audio/WildDiscovery.wav", true);  //music
    audio->playSource(menuMusic);
    
    models = new Models();
    obstacles = new Obstacles();
	obstacles->initialize(obstacleList);
	physics->newActors(obstacleList);	//adds the obstacles
    jeepManager = new JeepManager();
	jeepManager->initialize(physics, window.aInput);
    renderer->jeepManager = jeepManager; //update the pointer
    
    
}

void MainController::tickMenu(unsigned long interval)
{
    audio->pauseAllDynamicSources();
    audio->pauseSource(gameMusic);
    audio->playSource(menuMusic);
    if(!startMenu)
    {
        static std::string menus[] = {"data/UI/paused_0.png", "data/UI/paused_1.png", "data/UI/paused_2.png"};
        
        if(menuCount < 0)   //all the way up
        {
            menuCount = 2;
        }else if(menuCount > 2)   //all the way down
        {
            menuCount = 0;  //for now only 0 menus
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
        static std::string menus[] = {"data/UI/start_0.jpg"};
        
        menuCount = 0;
        if(menuSwitch)
        {
            renderer->setMessage(menus[0]);
            renderer->step();
            menuSwitch = false;
        }
        
    }
    
	//check user input
	static int keyCount = 0;    //big hack because our controller input is awesome
    keyCount += interval;
    if(keyCount < 50)
        return;    
	
	if(window.aInput->XAxis == -1)  //go left
	{
        keyCount = 0;
        menuCount --;
        menuSwitch = true;
	}
	
	if(window.aInput->XAxis == 1)  //go right
	{
        keyCount = 0;
        menuCount ++;
        menuSwitch = true;
	}
	
	if(window.aInput->Enter)
	{
	    if(menuCount == 0)
	    {
	            
            renderer->setMessage("");
            inMenu = false;
            audio->playAllDynamicSources();
            audio->pauseSource(menuMusic);
            
            //audio->playSource(gameMusic);
        }
        if(menuCount == 1)
        {
            restart();
            renderer->setMessage("");
            inMenu = false;
            audio->restartAllDynamicSources();
            audio->pauseSource(menuMusic);
            audio->restartSource(gameMusic);
            
        }
        
        if(menuCount == 2)
        {
            window.quit();
        }
	}
	
	//draw world and such so it's nice
    renderer->step();   //ensures a nice background
    jeepManager->renderTick();  //keep this in case it's paused	
    
    
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
	
	Sound * ptr = Sound::GetInstance();
    float t[] = {-behind[0], -behind[1], -behind[2], 0, 1, 0};
    ptr->setListener(pos, player->velocity, t);
    
	
	
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
    ptr->physics->restart();
    ptr->counting = true;
    ptr->countDown(0, true);    //restart countdown
	ptr->jeepManager->restart();
}

MainController::~MainController()
{
	for(ActorList::iterator itr = actorList.begin(); itr != actorList.end(); ++itr)
	{
		delete (*itr);
	}
	
    // delete models;
	
	delete physics;
	delete renderer;
}