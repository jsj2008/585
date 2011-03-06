#include "Physics.h"
#include "Common/SettingsFactory.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include <LinearMath/btIDebugDraw.h>
#include "HeightMap.h"
#include "HeightMapManager.h"
#include "Renderer/GLDebugDrawer.h"
#include "Common/Actor.h"

// #define DEBUG_RENDERING
// #define DEBUG_RENDERING2

Physics::Physics(ActorList const & actors, btIDebugDraw & debugger) : 
	actorList(actors), 
	debugger(debugger), 
	dispatcher(&collisionConfiguration), 
	dynamicsWorld(&dispatcher, &broadphase, &solver, &collisionConfiguration)
{	
		
	dynamicsWorld.setGravity(btVector3(0,LoadFloat("config/world.xml", "gravity"),0));   
	
	newActors(actors);
	
	/*turn on debugging*/
	#ifdef DEBUG_RENDERING
	debugger.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamicsWorld.setDebugDrawer(&debugger);
	#endif
	
	// HeightMap * m = new HeightMap(LoadString2("config/world.xml","height_map"));
	HeightMap const * m = HeightMapManager::GetHeightMap();
    btHeightfieldTerrainShape * heightfieldShape = new btHeightfieldTerrainShape(m->width, m->height,
					  m->map,
					  LoadFloat("config/world.xml","height_map_scale_y"),
					  -300, 300,
					  1, PHY_UCHAR, false);

	btTransform tr;
	tr.setIdentity();
	btVector3 localInertia(0,0,0);	
	
	heightfieldShape->setLocalScaling(btVector3(LoadFloat("config/world.xml","height_map_scale_x"), 1, LoadFloat("config/world.xml","height_map_scale_z")));

	btRigidBody* body = new btRigidBody(0,0,heightfieldShape,localInertia);	
	body->setWorldTransform(tr);

	dynamicsWorld.addRigidBody(body);
	

}

void Physics::newActors(ActorList const & newActors)
{
	for(ActorList::const_iterator itr = newActors.begin(); itr != newActors.end(); ++itr)
	{
		btVector3 vel = (*itr)->initialVel;
		Physics::MotionState * actorMotion = new Physics::MotionState( btTransform( btQuaternion(0,0,0,1), (*itr)->pos ), *itr);
		motionStates.push_back( actorMotion );
		
		PhysObject const & physObject = (*itr)->physObject;	//grabs physical info about the actor
		
		if(physObject.mass != 0)
			physObject.shape->calculateLocalInertia(physObject.mass, *(physObject.fallInertia) );	//dynamic object so calculate local inertia

		btRigidBody::btRigidBodyConstructionInfo bodyCI(physObject.mass, actorMotion, physObject.shape, *(physObject.fallInertia) );	//TODO this can be shared so stop recreating
		btRigidBody * body = new btRigidBody(bodyCI);
		dynamicsWorld.addRigidBody(body);
		
		body->setLinearVelocity(btVector3(vel.getX(),vel.getY(), vel.getZ()));
		rigidBodies.push_back(body);
		
	}
}

void Physics::step(seconds timeStep)
{
	dynamicsWorld.stepSimulation(timeStep,1);
	#ifdef DEBUG_RENDERING2
	dynamicsWorld.debugDrawWorld();
	#endif
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

		btVector3 vel = actor->initialVel;
		Physics::MotionState * actorMotion = new Physics::MotionState( btTransform( btQuaternion(0,0,0,1), actor->pos ), actor);
		motionStates.push_back( actorMotion );
		
		PhysObject const & physObject = actor->physObject;	//grabs physical info about the actor
		
		if(physObject.mass != 0)
			physObject.shape->calculateLocalInertia(physObject.mass, *(physObject.fallInertia) );	//dynamic object so calculate local inertia

		btRigidBody::btRigidBodyConstructionInfo bodyCI(physObject.mass, actorMotion, physObject.shape, *(physObject.fallInertia) );	//TODO this can be shared so stop recreating
		btRigidBody * body = new btRigidBody(bodyCI);
		dynamicsWorld.addRigidBody(body);
		
		body->setLinearVelocity(btVector3(vel.x(),vel.y(), vel.z() ));
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