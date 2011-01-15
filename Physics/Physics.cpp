#include "Physics.h"
#include <iostream>

Physics::Physics(ActorList * actors, GLDebugDrawer * debugger)
{
	this->actorList = actors;
	this->debugger = debugger;
	
	//broadphase method (how to check bounding boxes in non O(n^2) way)
	broadphase = new btDbvtBroadphase();	//using AABB method
	
	//once inside bounding box, how to do more sophisticated collision detection
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//solver makes sure objects interact with eachother and gravity, etc...
	solver = new btSequentialImpulseConstraintSolver();

	//construct the world
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
	//dynamicsWorld->setGravity(btVector3(0,-10,0));   
	dynamicsWorld->setGravity(btVector3(0,0,0));   //no gravity for debugging
	
	newActors(actors);
	
	/*turn on debugging*/
	dynamicsWorld->setDebugDrawer(debugger);
	debugger->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

}

void Physics::newActors(ActorList * newActors)
{
	
	for(ActorList::iterator itr = newActors->begin(); itr != newActors->end(); ++itr)
	{
		Point pos = (*itr)->pos;
		btMotionState * actorMotion = new Physics::MotionState( btTransform( btQuaternion(0,0,0,1), btVector3(pos.x, pos.y, pos.z) ), *itr);
		motionStates.push_back( actorMotion );
		
		PhysObject * physObject = (*itr)->physObject;	//grabs physical info about the actor
		
		if(physObject->mass != 0)
			physObject->shape->calculateLocalInertia(physObject->mass, *(physObject->fallInertia) );	//dynamic object so calculate local inertia

		btRigidBody::btRigidBodyConstructionInfo bodyCI(physObject->mass, actorMotion, physObject->shape, *(physObject->fallInertia) );	//TODO this can be shared so stop recreating
		btRigidBody * body = new btRigidBody(bodyCI);
		dynamicsWorld->addRigidBody(body);
		
		rigidBodies.push_back(body);
		
	}
}

void Physics::step(seconds timeStep)
{
	dynamicsWorld->stepSimulation(timeStep,10);	//keep an eye on the number of substeps (10 is pretty random)
	
	/*debugger*/
	debugger->startDebug();
	dynamicsWorld->debugDrawWorld();
	debugger->endDebug();
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
		dynamicsWorld->removeRigidBody( (*itr) );
		delete (*itr);
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