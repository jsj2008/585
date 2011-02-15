#ifndef PHYSICS_H
#define PHYSICS_H

/*all the libObjects*/
#include "PhysObject.h"
#include "libCube.h"
#include "libPlane.h"

#include "Common/prelude.h"
#include "Common/Actor.h"
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btIDebugDraw.h>
#include <list>

#include "Renderer/GLDebugDrawer.h"
#include "IPhysics.h"
class Spring;

class Physics : public IPhysics
{
public:
	Physics(ActorList const & actorList, btIDebugDraw & debugger);		//takes in list of actors (cars, falling objects etc...)
	~Physics();
	void newActors(ActorList const & newActors);	//physics needs to know if new objects have been added to actorList
	void step(seconds timeStep);
	btRigidBody * const newActor(Actor * const);
	friend class Spring;	
private:
	
	ActorList const & actorList;
	btIDebugDraw & debugger;
	btDbvtBroadphase broadphase;
	btDefaultCollisionConfiguration collisionConfiguration;
	btCollisionDispatcher dispatcher;
	btSequentialImpulseConstraintSolver solver;
	btDiscreteDynamicsWorld dynamicsWorld;

	
	class MotionState : public btMotionState	/*updates position of actors*/
	{
	public:
		MotionState(btTransform const & initalPos, Actor * actor);
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