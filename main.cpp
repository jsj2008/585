#include "Physics/Physics.h"
#include <QtGui>
#include "Common/Actor.h"

int main(int argc, char *argv[]) {
	
/*	QApplication a(argc, argv);
	Renderer r;
	r.show();
	
	
	return a.exec();*/
	
	ActorList * actors = new ActorList();
	
	actors->push_back(new Actor() );
	
	Physics phys(actors);
	
	
	/*remove actors*/
	for(ActorList::iterator itr = actors->begin(); itr != actors->end(); ++itr)
	{
		delete *itr;
	}
	
	delete actors;
	
	return 0;
}
