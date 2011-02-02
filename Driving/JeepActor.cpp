#include "JeepActor.h"
#include <iostream>
#include "Common/SettingsFactory.h"

JeepActor::JeepActor(PhysObject const & physObj, Physics * const physics, Point pos, Point vel) : Actor::Actor(physObj, pos, vel),
physics(physics)
{
	isForward = false;
	isBackward = false;
	
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
	
	float const & offset_x = LoadFloat("config/jeep_springs.xml", "offset_x");
	float const & offset_z = LoadFloat("config/jeep_springs.xml", "offset_z");
	float const & spring_top = LoadFloat("config/jeep_springs.xml", "spring_top");
	float const & spring_bottom = LoadFloat("config/jeep_springs.xml", "spring_bottom");	
	
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
	// chasis->applyImpulse(btVector3(150, 30.5, 0), btVector3(0.1,0,0));

	for(int i=0; i<4; i++)
		springs.push_back(new Spring(chasis, from[i], to[i], physics) );
}

void JeepActor::moveForward( bool isDown)
{
	isForward = isDown;
}

void JeepActor::moveBackward( bool isDown)
{
	isBackward = isDown;
}

void JeepActor::tick(seconds timeStep)
{
	for(int i=0; i<4; i++)
	{
		springs[i]->tick(timeStep, pos);	/*apply springs*/
	}
	
	/*calculate constants*/
	btVector3 u = quatRotate(chasis->getOrientation(), btVector3(1,0,0));
	// btVector3 v = quatRotate(chasis->getOrientation(), btVector3(-0.7, -1.0, 0));
	btVector3 velocity = u*chasis->getLinearVelocity().dot(u);	//do a projection in direction we are travelling
	btScalar speed = velocity.length();

	
	/*hacking user input*/
	btScalar engine_force = 0;
	if(isForward)
	{
		engine_force = 2;
	}
	
	if(isBackward)
	{
		engine_force = -2;	//for now just make it one or the other
		//f_breaking = -u * c_breaking;
		
	}
	
	//rear wheel driving
	btVector3 f = springs[0]->getForce(engine_force, chasis->getLinearVelocity(), u );
	chasis->applyCentralForce( f );
	
	//btVector3 f_traction = u * engine_force;	//simple engine for now
	
	/*air resistance*/
	 btScalar c_drag = 2.0;
	 btVector3 f_drag = -c_drag * velocity * speed;
	
	/*rolling resistance*/
	btScalar c_rolling = 30 * c_drag;
	btVector3 f_rolling = -c_rolling * velocity;
	
	// chasis->applyCentralForce( f_drag + f_rolling );
		
	
	/*apply load*/
	// btScalar rear_weight = (springs[0]->getWeight() + springs[1]->getWeight()) / 2.0;	//assuming rear wheel drive
	
	// btVector3 f_long = u*rear_weight / 10 * slip_ratio ;//* rear_weight;
	
	/*output longitudinal force*/
	// f_long += f_drag + f_rolling + f_breaking;
	
	
	/*apply forces*/
	// std::cout << slip_ratio << " " << f_long.x() << std::endl;
	// chasis->applyForce(f_long, btVector3(0,0,0));
	
	
	/*lateral forces*/
	//btScalar c_a = 1;
	
	//btVector3 f_lateral = c_a * alpha
	
	/*calculate needed constants*/
	/*
	
	btScalar back_weight = (springs[0]->getWeight() + springs[1]->getWeight()) / 2.0;
	std::cout << back_weight << std::endl;
	

	btScalar static_coeff = 1.5;
	btScalar kinetic_coeff = 1.0;
	btScalar static_friction =  static_coeff*back_weight;
	btVector3 friction;
	btScalar rolling_coeff = 10;
	btVector3 rolling_friction = -linear_velocity * rolling_coeff;
	
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
	/*	std::cout << "pos_x:" << pos.x() << std::endl;
	btVector3 traction = btVector3(0,0,0);
	if(isForward)
	{
		//chasis->applyForce(btVector3(150,0,0), btVector3(-1,-1,0));
		//traction = btVector3(150, 0, 0);
	}
	
	btScalar wheel_radius = 0.25;
	btScalar wheel_angular_velocity = speed / (2.0 * 3.14159265);	// \omega = |v| / 2pi
	btScalar slip_ratio = 0;
	
	if(speed > 0)	//car is moving so calculate slip ratio
		slip_ratio = (wheel_angular_velocity * wheel_radius - speed ) / fabs(speed);
	
	btVector3 traction_force = btVector3(1, 0, 0) * slip_ratio;
	chasis->applyCentralForce(traction + rolling_friction);*/
	
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
