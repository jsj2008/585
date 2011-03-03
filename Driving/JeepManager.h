#ifndef JEEPMANAGER_H
#define JEEPMANAGER_H

#include "JeepActor.h"
#include "Physics/Physics.h"

class JeepManager
{
public:
	typedef std::list<JeepActor *> Jeeps;
	JeepManager(Jeeps *, Physics *);
	static void tickCallback(btDynamicsWorld *world, btScalar timeStep);

private:
	Jeeps * jeeps;
	
};

#endif