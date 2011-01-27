#include "Physics.h"
#include <iostream>

Physics::Physics(ActorList const & actors, btIDebugDraw & debugger) : 
	actorList(actors), debugger(debugger), dispatcher(&collisionConfiguration), dynamicsWorld(&dispatcher, &broadphase, &solver, &collisionConfiguration)
{	
		
	dynamicsWorld.setGravity(btVector3(0,-10,0));   
	
	newActors(actors);
	
	/*turn on debugging*/
	debugger.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamicsWorld.setDebugDrawer(&debugger);

}

void Physics::newActors(ActorList const & newActors)
{
	for(ActorList::const_iterator itr = newActors.begin(); itr != newActors.end(); ++itr)
	{
		Point vel = (*itr)->initialVel;
		Physics::MotionState * actorMotion = new Physics::MotionState( btTransform( btQuaternion(0,0,0,1), (*itr)->pos ), *itr);
		motionStates.push_back( actorMotion );
		
		PhysObject const & physObject = (*itr)->physObject;	//grabs physical info about the actor
		
		if(physObject.mass != 0)
			physObject.shape->calculateLocalInertia(physObject.mass, *(physObject.fallInertia) );	//dynamic object so calculate local inertia

		btRigidBody::btRigidBodyConstructionInfo bodyCI(physObject.mass, actorMotion, physObject.shape, *(physObject.fallInertia) );	//TODO this can be shared so stop recreating
		btRigidBody * body = new btRigidBody(bodyCI);
		dynamicsWorld.addRigidBody(body);
		
		body->setLinearVelocity(btVector3(vel.x,vel.y, vel.z));
		rigidBodies.push_back(body);
		
	}
}

void Physics::step(seconds timeStep)
{
	dynamicsWorld.stepSimulation(timeStep,10);	//keep an eye on the number of substeps (10 is pretty random)
	dynamicsWorld.debugDrawWorld();
	
	btVector3 from[4];
	btVector3 to[4];
/*	vehicle->getRays(from, to);
	static float old_x[4] = {0,0,0,0};
	static float k=50.0;
	static float mass = 10.0;
	static float c = 1.5*sqrt(k/mass);
	
	
	
	
	/*for(int i=0; i<4; i++)
	{
		btCollisionWorld::ClosestRayResultCallback result(from[i], to[i]);
		
		dynamicsWorld.rayTest( from[i], to[i], result);
		btVector3 rest = to[i];
		debugger.drawLine(from[i],rest, btVector3(255,0,255));
		debugger.drawLine(from[i],to[i] ,btVector3(255,255,255));
		if(result.hasHit() )
		{		
			btScalar x = (rest - result.m_hitPointWorld).length();
			for(RigidBodies::iterator itr = rigidBodies.begin(); itr != rigidBodies.end(); ++itr)
			{
				//std::cout << x << std::endl;
				btVector3 unit = to[i] - from[i];
				unit.normalize();
				(*itr)->applyForce( -unit * (k* x - c*(abs(x - old_x[i]))/timeStep), from[i]);
			}
			old_x[i] = x;
			
			//offset[i] = (x - offset[i]) / (timeStep * 100);
		}else
		{
			old_x[i] = 0;
		}
	}*/
}

Physics::~Physics()
{
	//get rid of all the bullet stuff
	
	for(MotionStates::iterator itr = motionStates.begin(); itr != motionStates.end(); ++itr)
	{
		delete (*itr);	//remove all motion states
	}
	
	for(RigidBodies::iterator itr = rigidBodies.begin(); itr != rigidBodies.end(); ++itr)
	{
		dynamicsWorld.removeRigidBody( (*itr) );
		delete (*itr);
	}
}

btRigidBody * const Physics::newActor(Actor * const actor)
{

		Point vel = actor->initialVel;
		Physics::MotionState * actorMotion = new Physics::MotionState( btTransform( btQuaternion(0,0,0,1), actor->pos ), actor);
		motionStates.push_back( actorMotion );
		
		PhysObject const & physObject = actor->physObject;	//grabs physical info about the actor
		
		if(physObject.mass != 0)
			physObject.shape->calculateLocalInertia(physObject.mass, *(physObject.fallInertia) );	//dynamic object so calculate local inertia

		btRigidBody::btRigidBodyConstructionInfo bodyCI(physObject.mass, actorMotion, physObject.shape, *(physObject.fallInertia) );	//TODO this can be shared so stop recreating
		btRigidBody * body = new btRigidBody(bodyCI);
		dynamicsWorld.addRigidBody(body);
		
		body->setLinearVelocity(btVector3(vel.x,vel.y, vel.z));
		rigidBodies.push_back(body);
		
		return body;
}

Physics::MotionState::MotionState(const btTransform &initialPos, Actor * actor)
{
	this->actor = actor;
	this->pos = initialPos;
}

Physics::MotionState::~MotionState(){}

void Physics::MotionState::getWorldTransform(btTransform &worldTrans) const
{
	worldTrans = pos;
}

void Physics::MotionState::setWorldTransform(const btTransform &worldTrans)
{
	actor->setOrientation(worldTrans.getRotation());
	actor->setPosition(worldTrans.getOrigin());
}