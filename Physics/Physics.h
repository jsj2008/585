#ifndef PHYSICS_H
#define PHYSICS_H

#include <btBulletDynamicsCommon.h>
#include "IPhysics.h"

class Spring;
class JeepActor;
class JeepManager;
class Actor;

class Physics : public IPhysics
{
public:
	Physics(ActorList const & actorList, btIDebugDraw & debugger);		//takes in list of actors (cars, falling objects etc...)
	~Physics();
	void newActors(ActorList const & newActors);	//physics needs to know if new objects have been added to actorList
	void step(btScalar timeStep);
    void restart();
	btRigidBody * const newActor(Actor * const);
	friend class Spring;
	friend class JeepActor;
	friend class JeepManager;
private:
	
	ActorList const & actorList;
	btIDebugDraw & debugger;
	btDbvtBroadphase broadphase;
	btDefaultCollisionConfiguration collisionConfiguration;
	btCollisionDispatcher dispatcher;
	btSequentialImpulseConstraintSolver solver;
	btDiscreteDynamicsWorld dynamicsWorld;
	// btContinuousDynamicsWorld dynamicsWorld;

	
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