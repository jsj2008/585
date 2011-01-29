#include "JeepActor.h"
#include <iostream>

JeepActor::JeepActor(PhysObject const & physObj, Physics * const physics, Point pos, Point vel) : Actor::Actor(physObj, pos, vel),
physics(physics)
{
	/*set up the springs
	
	3.0 x 0.3 x 2.0
	
	0----------1
	|          |
	|          |
	|          |
	|          |
	|          |
	2----------3
	
	0 is front left, 1 is back left, 2 is front right, 3 is back right
	*/
	
	float offset_z = 0.9;
	float offset_x = 1.5;
	float spring_top = 0;
	float spring_bottom = -1;
	
	origin_from[0] = 	btVector3(-offset_x, spring_top, offset_z);
	from[0] = origin_from[0];
	origin_from[1] = 	btVector3(-offset_x, spring_top, -offset_z);
	from[1] = origin_from[1];
	origin_from[2] =	btVector3(offset_x, spring_top, offset_z);
	from[2] = origin_from[2];
	origin_from[3] =	btVector3(offset_x, spring_top, -offset_z);
	from[3] = origin_from[3];

	origin_to[0] = btVector3(-offset_x, spring_bottom, offset_z);
	//origin_to[0] = btVector3(0, -2.0, 0);
	to[0] = origin_to[0];
	origin_to[1] =	btVector3(-offset_x, spring_bottom, -offset_z);
	to[1] = origin_to[1];
	origin_to[2] =	btVector3(offset_x, spring_bottom, offset_z);
	to[2] = origin_to[2];
	origin_to[3] =	btVector3(offset_x, spring_bottom, -offset_z);
	to[3] = origin_to[3];
	
	chasis = physics->newActor(this);
	// chasis->applyCentralImpulse(btVector3(0, -5.5, 0));

	for(int i=0; i<4; i++)
		springs.push_back(new Spring(chasis, from[i], to[i], physics) );
}

void JeepActor::tick(seconds timeStep)
{
	static btScalar a = 0;
	//btVector3 position(pos.x, pos.y, pos.z);
	
	for(int i=0; i<4; i++)
	{
		std::cout << i << std::endl;
		springs[i]->tick(timeStep, pos);
	}

	
	btVector3 linear_velocity = chasis->getLinearVelocity();
	btVector3 static_friction = -linear_velocity * 10;
	
	/*btScalar wheel_weight = (springs[0]->getX() + springs[1]->getX()) / 2.0;
	if(wheel_weight < 5)
	{
		a = 20 - 5.5* wheel_weight;
		static_friction -= btVector3(1,0,0)*wheel_weight;
	}else
	{
		a = 0;
		static_friction *= 0;
	}*/
	
	// a = 0;
	
	btVector3 traction = btVector3(0, 0, 0);
	chasis->applyCentralForce(traction + static_friction);
	
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
