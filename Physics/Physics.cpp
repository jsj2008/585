#include "Physics.h"
#include <iostream>

Physics::Physics(ActorList * actors)
{
	this->actorList = actors;
	
	//broadphase method (how to check bounding boxes in non O(n^2) way)
	broadphase = new btDbvtBroadphase();	//using AABB method
	
	//once inside bounding box, how to do more sophisticated collision detection
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//solver makes sure objects interact with eachother and gravity, etc...
	solver = new btSequentialImpulseConstraintSolver;

	//construct the world
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0,-10,0));   
	
	newActors(actors);
    

}

void Physics::newActors(ActorList * newActors)
{
	for(ActorList::iterator itr = newActors->begin(); itr != newActors->end(); ++itr)
	{
		Point pos = (*itr)->pos;
		motionStates.push_back(new MotionState( btTransform( btQuaternion(pos.x,pos.y,pos.z,1), btVector3(0,0,1) ), *itr) );
	}
}

Physics::~Physics()
{
	//get rid of all the bullet stuff
	
	for(MotionStates::iterator itr = motionStates.begin(); itr != motionStates.end(); ++itr)
	{
		delete (*itr);	//remove all motion states
	}
	
	delete broadphase;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
	delete dynamicsWorld;
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
	btQuaternion rot = worldTrans.getRotation();
	//actor->setRotation(rot)
	btVector3 pos = worldTrans.getOrigin();
	//actor->setPosition(pos)	//TODO use something like this which isn't terrible (threadsafe?)
	actor->pos.x = pos.getX();
	actor->pos.y = pos.getY();
	actor->pos.z = pos.getZ();
}