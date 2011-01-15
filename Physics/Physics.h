#ifndef PHYSICS_H
#define PHYSICS_H

#include "../Common/prelude.h"
#include "../Common/Actor.h"
#include <btBulletDynamicsCommon.h>


class Physics
{
public:
	Physics(ActorList * actorList);		//takes in list of actors (cars, falling objects etc...)
	~Physics();
	void newActors(ActorList * newActors);	//physics needs to know if new objects have been added to actorList
private:
	ActorList * actorList;
	btBroadphaseInterface * broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	
	btCollisionShape* ground;	//TODO this is just temporary plane
	btDefaultMotionState* groundMotionState;
	
	btCollisionShape* sphere;
	
	void setupPrimitives();

};

#endif