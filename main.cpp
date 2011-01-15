#include "Physics/Physics.h"
#include <QtGui>
#include "Common/Actor.h"
#include "Renderer/Renderer.h"
#include "Renderer/GLDebugDrawer.h"
#include <iostream>

int main(int argc, char *argv[]) {
	
	
	
	
	
	QApplication a(argc, argv);

	Renderer r;
	GLDebugDrawer debug(&r);
	
	ActorList * actors = new ActorList();
	
	actors->push_back(new Actor() );
	
	Physics phys(actors, &debug);


	r.show();
	
	for(int i=0; i<10; i++)
	{
		phys.step(1);
	}	
	
	return a.exec();

	/*remove actors*/
	for(ActorList::iterator itr = actors->begin(); itr != actors->end(); ++itr)
	{
		delete *itr;
	}

	delete actors;	
}
