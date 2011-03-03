#include "JeepManager.h"

JeepManager::JeepManager(Jeeps * jeeps, Physics * physics)
{
	this->jeeps = jeeps;
	physics->dynamicsWorld.setInternalTickCallback(tickCallback, static_cast<void *>(this), true );
}

void JeepManager::tickCallback(btDynamicsWorld *world, btScalar timeStep)
{
	JeepManager * m = static_cast<JeepManager *>(world->getWorldUserInfo());
	
	for(Jeeps::iterator itr = m->jeeps->begin(); itr != m->jeeps->end(); ++itr)
		(*itr)->tick(timeStep);
}
