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
pos(btVector3(0,0,0)),
finished(false),
inCar(false)
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
	readySource = audio->addSource("data/audio/ready.wav", true);
    audio->playSource(menuMusic);
    
    models = new Models();
    obstacles = new Obstacles();
	obstacles->initialize(obstacleList);
	physics->newActors(obstacleList);	//adds the obstacles
    jeepManager = new JeepManager();
	jeepManager->initialize(physics, window.aInput);
    renderer->jeepManager = jeepManager; //update the pointer
    
    
}

void MainController::finishGame( std::vector<int> & places)
{
	ptr->inMenu = true;
	ptr->finished = true;
	ptr->menuSwitch = true;
	ptr->startMenu = false;
	ptr->renderer->finishGame();
	ptr->menuCount = 3;

	ptr->place = 1;
	for(std::vector<int>::iterator itr = places.begin(); itr != places.end(); ++itr, ptr->place++)
	{
		if(*itr == LoadInt("config/ai.xml", "num_players"))	//the player
		{
			break;
		}
	}

}

void MainController::tickMenu(unsigned long interval)
{
    audio->pauseAllDynamicSources();
    audio->pauseSource(gameMusic);
    audio->playSource(menuMusic);
	audio->pauseSource(readySource);

	if(startMenu)
	{
		static std::string menus[] = {"data/UI/start_new.png", "data/UI/story_new.png", "data/UI/credits_new.png", "data/UI/quit_new.png"};
		if(menuCount < 0)
		{
			menuCount = 3;
		}else if(menuCount > 3)
		{
			menuCount = 0;
		}

		if(menuSwitch)
		{
			renderer->setMessage(menus[menuCount]);
            menuSwitch = false;
		}

	}else
	{
		static std::string menus[] = {"data/UI/paused_resume.png", "data/UI/paused_restart.png", "data/UI/paused_quit.png", "data/UI/complete_restart.png", "data/UI/complete_quit.png"};
		if(finished)
		{
			if(menuCount < 3)
			{
				menuCount = 4;
			}else if(menuCount > 4)
			{
				menuCount = 3;
			}
		}else
		{
			if(menuCount < 0)
			{
				menuCount = 2;
			}else if(menuCount > 2)
			{
				menuCount = 0;
			}
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
    	pos += (look+ 25*behind - pos ) / 5.0;

    	renderer->setCamera(pos,look);

	}
    
	//check user input
	static int lastX = 0;
	if(window.aInput->XAxis == 1 || window.aInput->XAxis == -1)
	{
		//draw world and such so it's nice
		 	lastX = window.aInput->XAxis;
		if(lastX != 0)
		{
			renderer->step();   //ensures a nice background
			jeepManager->renderTick();  //keep this in case it's paused  
	
			return;
		}
	}

	if(lastX <0)  //go left
	{
        lastX = 0;
        menuCount --;
        menuSwitch = true;
	}
	
	if(lastX >0)  //go right
	{
        lastX = 0;
        menuCount ++;
        menuSwitch = true;
	}
	
	if(window.aInput->Enter)
	{
		if(startMenu)
		{
			if(menuCount == 0)
			{
				restart();
				jeepManager->startEngines();
				renderer->startGame();

				renderer->setMessage("");
				inMenu = false;
				audio->playAllDynamicSources();
				audio->pauseSource(menuMusic);
				audio->resumeSource(readySource);
				audio->playSource(gameMusic);
				wasOut = true;
			}

			if(menuCount == 3)
			{
				window.quit();
			}




		}else
		{
			if(menuCount == 0)
			{
				renderer->setMessage("");
				inMenu = false;
				audio->playAllDynamicSources();
				audio->pauseSource(menuMusic);
				audio->resumeSource(readySource);
				audio->playSource(gameMusic);
				wasOut = true;
			}

			if(menuCount == 1 || menuCount == 3)
			{
				restart();
				inMenu = false;
				audio->playAllDynamicSources();
				audio->pauseSource(menuMusic);
				audio->restartSource(gameMusic);
				wasOut = true;
				finished = false;
				renderer->startGame();
			}

			if(menuCount == 2 || menuCount == 4)
			{
				startMenu = true;
				menuSwitch = true;
				menuCount = 2;
			}

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
        //std::cout << "LAG" << std::endl;
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

	static btVector3 pos2 = btVector3(0,0,0);
	JeepActor* player = jeepManager->getHuman();
	btVector3 look = player->pos;
	btVector3 behind = quatRotate(player->orientation, btVector3(-0.8,0.4, 0) );
	btVector3 behind2 = quatRotate(player->orientation, btVector3(0.8,0.4, 0) );
	btVector3 front = quatRotate(player->orientation, btVector3(1.0,-0.3, 0) );
	btVector3 up = quatRotate(player->orientation, btVector3(0.0,1.0, 0) );
	static btVector3 lookUp = btVector3(0,1,0);
	pos += (look+ 25*behind - pos ) / 10.0;
	pos2 = look + 25*behind2;
	//lookUp += (up - lookUp) / 10.0;
	lookUp = up;
	
	Sound * ptr = Sound::GetInstance();
    
	//switch cameras
	static int keyCool = 0;
	keyCool += 1;
	if(keyCool > 50 && window.aInput->BButton)
	{
		inCar = !inCar;
		keyCool = 0;
	}

	if(window.aInput->LButton)
	{
		float t[] = {-behind2[0], -behind2[1], -behind2[2], 0, 1, 0};
		ptr->setListener(pos2, player->velocity, t);
    
		renderer->setCamera(pos2,look);
		
		if(!wasOut)
			renderer->setMessage("");
		wasOut = true;
	}
	else if(inCar)
	{
		btVector3 pos3 = look + 3.5*up + 1.4*front;
		float t[] = {player->u[0], player->u[1], player->u[2], lookUp[0], lookUp[1], lookUp[2]};
		ptr->setListener(pos3, player->velocity, t);

		renderer->setCamera(pos3, look + 25*front, lookUp);

		if(wasOut)
			renderer->setMessage("data/windshield.png");

		wasOut = false;
	}else
	{
		float t[] = {-behind[0], -behind[1], -behind[2], 0, 1, 0};
		ptr->setListener(pos, player->velocity, t);

		renderer->setCamera(pos,look);
		
		if(!wasOut)
			renderer->setMessage("");
		wasOut = true;
	}
	
	
	
	
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
    static std::string imgs[] = {"", "data/UI/0.png", "data/UI/1.png", "data/UI/2.png"};
    static int timer = 0;
    static int count = 3;

    if(restart)
    {
        timer = 0;
        count = 3;
    }
    
    timer += interval;
    if(timer > 1000)
    {	
		if(count == 3)	//start
			audio->playSource(readySource);
        //std::cout << "count" << std::endl;
		if(!inCar)	//don't show if inside the car
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

void MainController::restartJeep(JeepActor * jeep)
{
	ptr->jeepManager->restartJeep(jeep);
}

void MainController::restart()
{
	ptr->inCar = false;
    ptr->physics->restart();
    ptr->counting = true;
    ptr->countDown(0, true);    //restart countdown
	ptr->jeepManager->restart();
}

ActorList const * MainController::getObstacleList()
{
    return &(ptr->obstacleList);
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