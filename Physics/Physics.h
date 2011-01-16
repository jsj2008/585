#ifndef PHYSICS_H
#define PHYSICS_H

/*all the libObjects*/
#include "physObject.h"
#include "libCube.h"
#include "libPlane.h"

#include "../Common/prelude.h"
#include "../Common/Actor.h"
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btIDebugDraw.h>
#include <list>

#include "../Renderer/GLDebugDrawer.h"


class Physics
{
public:
	Physics(ActorList * actorList, GLDebugDrawer * debugger);		//takes in list of actors (cars, falling objects etc...)
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
	GLDebugDrawer * debugger;
	
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