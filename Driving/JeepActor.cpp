#include "JeepActor.h"
#include <iostream>

JeepActor::JeepActor(PhysObject const & physObj, Physics * const physics, Point pos, Point vel) : Actor::Actor(physObj, pos, vel),
physics(physics)
{
	/*set up the springs
	
	3.0 x 0.3 x 1.0
	
	0----------1
	|          |
	|          |
	|          |
	|          |
	|          |
	2----------3
	
	0 is front left, 1 is back left, 2 is front right, 3 is back right
	*/
	
	origin_from[0] = 	btVector3(-1.5, -0.05, 1.5);
	from[0] = origin_from[0];
	origin_from[1] = 	btVector3(-1.5, -0.05, -1.5);
	from[1] = origin_from[1];
	origin_from[2] =	btVector3(1.5, -0.05, 1.5);
	from[2] = origin_from[2];
	origin_from[3] =	btVector3(1.5, -0.05, -1.5);
	from[3] = origin_from[3];

	origin_to[0] = btVector3(-1.5, -2.0, 1.5);
	to[0] = origin_to[0];
	origin_to[1] =	btVector3(-1.5, -2.0, -1.5);
	to[1] = origin_to[1];
	origin_to[2] =	btVector3(1.5, -2.0, 1.5);
	to[2] = origin_to[2];
	origin_to[3] =	btVector3(1.5, -2.0, -1.5);
	to[3] = origin_to[3];
	
	btRigidBody * body = physics->newActor(this);
	btTransform com = btTransform::getIdentity();
	com.setOrigin(btVector3(0, -0, 0));
	body->setCenterOfMassTransform(com);
	std::cout << "Center of Mass:" << com.getOrigin().getZ();
	for(int i=0; i<4; i++)
		springs.push_back(new Spring(body, from[i], to[i], physics) );
}

void JeepActor::tick(seconds timeStep)
{
	for(int i=0; i<4; i++)
	{
		springs[i]->tick(timeStep);
	}
}

JeepActor::~JeepActor()
{
	for(int i=0; i<4; i++)
	{
		delete springs[i];
	}
}

void JeepActor::setOrientation(btQuaternion rot)
{
	Actor::setOrientation(rot);
	/*update spring orientation*/
	for(int i=0; i<4; i++)
	{
		from[i] = quatRotate(rot, origin_from[i]);
		to[i] = quatRotate(rot, origin_to[i]);
		from[i] += pos;
		to[i] += pos;
	}
}

void JeepActor::setPosition(btVector3 pos)
{
	Actor::setPosition(pos);
	
	/*update spring pos*/
	for(int i=0; i<4; i++)
	{
		from[i] = quatRotate(orientation, origin_from[i]);
		to[i] = quatRotate(orientation, origin_to[i]);
		from[i] += pos;
		to[i] += pos;
	}}
