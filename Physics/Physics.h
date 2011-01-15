#ifndef PHYSICS_H
#define PHYSICS_H

#include "../Common/prelude.h"
#include "../Common/Actor.h"
#include <btBulletDynamicsCommon.h>
#include <list>


class Physics
{
public:
	Physics(ActorList * actorList);		//takes in list of actors (cars, falling objects etc...)
	~Physics();
	void newActors(ActorList * newActors);	//physics needs to know if new objects have been added to actorList
	void step(seconds timeStep);
private:
	ActorList * actorList;
	btBroadphaseInterface * broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	
	class MotionState : public btMotionState	/*updates position of actors*/
	{
	public:
		MotionState(const btTransform &initalPos, Actor * actor);
		virtual ~MotionState();
		virtual void getWorldTransform(btTransform &worldTrans) const;
		virtual void setWorldTransform(const btTransform &worldTrans);
	private:
		btTransform pos;
		Actor * actor;
	};
	
	typedef std::list<btMotionState*> MotionStates;
	typedef std::list<btRigidBody*> RigidBodies;

	MotionStates motionStates;
	RigidBodies rigidBodies;
	

};

#endif