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
	
	setupPrimitives();
   
    

}

void Physics::setupPrimitives()
{
	ground = new btStaticPlaneShape(btVector3(0,1,0),1);	//sets up temporary plane
    sphere = new btSphereShape(1);	//sets up sphere shape

	groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
	
}

void Physics::newActors(ActorList * newActors)
{
	
}

Physics::~Physics()
{
	//get rid of all the bullet stuff
	delete broadphase;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
	delete dynamicsWorld;
}