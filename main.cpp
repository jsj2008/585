#include "Main/MainController.h"
#include "Physics/Physics.h"
#include "Physics/libCube.h"
#include "Physics/libPlane.h"
#include <QtGui>
#include "Common/Actor.h"
#include "Renderer/Renderer.h"
#include "Renderer/GLDebugDrawer.h"
#include <iostream>

int main(int argc, char *argv[]) {
	
	
	
	/*shapes*/
	PhysObject * cube = new libCube();
	PhysObject * plane= new libPlane();
	
	
	QApplication a(argc, argv);

	Renderer r;

	GLDebugDrawer debug(&r);
	
	ActorList * actors = new ActorList();

	for(int i=0; i<10; i++)
	{
		Actor * act = new Actor( 1.0 / (rand() % 10 + 1), 2*i + 1, 0 );
		act->physObject = cube;
		actors->push_back(act);
	}
	
	Actor * act = new Actor(0, -5, 0 );
	act->physObject = plane;
	actors->push_back(act);
	

	Physics phys(actors, &debug);

	MainController m(&r, &phys);
	r.setParent(&m);
	
	m.show();
	
	
	return a.exec();

	/*remove actors*/
	/*for(ActorList::iterator itr = actors->begin(); itr != actors->end(); ++itr)
	{
		delete *itr;
	}

	delete actors;	*/
}
