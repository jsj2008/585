#include "MainController.h"
#include "Physics/PhysicsFactory.h"
#include <iostream>
#include "Common/SettingsFactory.h"

#include <irrKlang.h>
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
using namespace irrklang;

MainController * MainController::ptr = NULL;


MainController::MainController() : 
physics(PhysicsFactory::newPhysics(actorList, debugger) ),
jeepModel("", "", "models/jeep2_flipx.obj"),
cubeModel("RAADicle.bmp", "", "models/cube.obj"),
planeModel("RAADicleXtreme.bmp", "", "models/quad.obj")

{
	if(ptr == NULL)
		ptr = this;

	float const & planeY = LoadFloat("config/start.xml", "planeY");
	float const & jeepX = LoadFloat("config/start.xml", "jeepX");
	float const & jeepY = LoadFloat("config/start.xml", "jeepY");
	float const & jeepZ = LoadFloat("config/start.xml", "jeepZ");

		//renderTest = RenderObject("testBox.bmp", "ducky.obj");

	ActorList temp;
	 // Actor * act = new Actor(mPlane, planeModel, btVector3(0,-5,0));
	 // actorList.push_back(act);	

	/***********Sound test********/ 
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine) {
		std::cout << "Could not startup sound engine\n" << std::endl;
	} else {
		engine->play2D("sound/TribalGroove.mp3", true);
		engine->play2D("sound/ExoticBirds.mp3", true);
		//engine->play3D("sound/FX/test.wav", vec3df(0,0,2));
	}
	/***********Sound test********/ 
	

	 // temp.push_back(act);
		
	/*pass jeep into physics/renderer but don't add to dynamicWorld (this is done by jeep internally)*/
	aiInput = new AIInput();
	jeep = new JeepActor(mChasis, jeepModel, physics, aiInput, btVector3(jeepX, jeepY, jeepZ));
	//jeep2 = new JeepActor(mChasis, jeepModel, physics, aiInput, btVector3(jeepX, jeepY, jeepZ+20));
	/*jeep3 = new JeepActor(mChasis, jeepModel, physics, window.bInput , btVector3(jeepX, jeepY, jeepZ+40));
	jeep4 = new JeepActor(mChasis, jeepModel, physics, window.bInput , btVector3(jeepX, jeepY, jeepZ+60));
	jeep5 = new JeepActor(mChasis, jeepModel, physics, window.bInput , btVector3(jeepX, jeepY, jeepZ+80));
	jeep6 = new JeepActor(mChasis, jeepModel, physics, window.bInput , btVector3(jeepX, jeepY, jeepZ+100));*/
	
	jeeps.push_back(jeep);
	//jeeps.push_back(jeep2);
	/*jeeps.push_back(jeep3);
	jeeps.push_back(jeep4);
	jeeps.push_back(jeep5);
	jeeps.push_back(jeep6);*/
	
	jeepManager = new JeepManager(&jeeps, physics);

	actorList.push_back(jeep);
	//actorList.push_back(jeep2);	
	/*actorList.push_back(jeep3);	
	actorList.push_back(jeep4);	
	actorList.push_back(jeep5);	
	actorList.push_back(jeep6);	*/
		
	/*setup subcomponents*/
	physics->newActors(temp);
	renderer = new Renderer(window, actorList);
	levelAI = new LevelAI(jeep);
	aiInput->setLevelAI(levelAI);

	window.run(this);	//launch window

}

void MainController::yield()
{
	
}

void MainController::tick(unsigned long interval)
{
	//std::cout << interval << std::endl;
	renderer->step();
	levelAI->step();
	aiInput->step();
	physics->step( interval / 1000.0);
	jeep->render();
	//jeep2->render();
	/*jeep3->render();
	jeep4->render();
	jeep5->render();
	jeep6->render();*/
	
	/*giant hack for camera*/

	static btVector3 pos = btVector3(9,11,15);
	btVector3 look = jeep->pos;
	btVector3 behind = quatRotate(jeep->orientation, btVector3(-1,0.4,0) );
	pos += (look + 40*behind - pos ) / 30.0;
	
	renderer->setCamera(pos,look);
	
}

void MainController::explode()
{
	static int counter = 0;
	ActorList temp;
	Real rad = 3.1415926 / 6 * counter++;	//pick random angle
	
	Actor * act = new Actor(mCube, cubeModel, btVector3(0,-3, 0), btVector3(cos(rad)*5, 10, sin(rad)*5 ) );
	actorList.push_back(act);
	temp.push_back(act);
		
	physics->newActors(temp);
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
	delete levelAI;
}