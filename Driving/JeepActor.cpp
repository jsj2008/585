#include "JeepActor.h"
#include <iostream>
#include "Common/SettingsFactory.h"

JeepActor::JeepActor(PhysObject const & physObj, Physics * const physics, Point pos, Point vel) : Actor::Actor(physObj, pos, vel),
physics(physics), offset_x(LoadFloat("config/jeep_springs.xml", "offset_x")), offset_z(LoadFloat("config/jeep_springs.xml", "offset_z")),
spring_top(LoadFloat("config/jeep_springs.xml", "spring_top")), spring_bottom(LoadFloat("config/jeep_springs.xml", "spring_bottom")),
mass(LoadFloat("config/jeep_springs.xml", "mass") )
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
	static float torque = 0;
	static float gravity = 1.0;
	static float const & torque_k = LoadFloat("config/jeep_springs.xml", "torque_k");
	static float const & comx= LoadFloat("config/jeep_springs.xml", "comx");
	static float const & comz = LoadFloat("config/jeep_springs.xml", "comz");
	static float const & c_drag = LoadFloat("config/jeep_springs.xml", "c_drag");
	static float const & c_roll = LoadFloat("config/jeep_springs.xml", "c_roll");
	static float const & breaking = LoadFloat("config/jeep_springs.xml", "breaking");
	static float const & engine_torque = LoadFloat("config/jeep_springs.xml", "engine_torque");
	static float const & weight_shift = LoadFloat("config/jeep_springs.xml", "weight_shift");
	static btScalar weight_front = 0;
	static btScalar weight_rear = 0;
	for(int i=0; i<4; i++)
	{
		springs[i]->tick(timeStep, pos);	/*apply springs*/
	}
	
	/*calculate constants*/
	btVector3 u = quatRotate(chasis->getOrientation(), btVector3(1,0,0) );
	btVector3 front_tire = quatRotate(chasis->getOrientation(), btVector3(offset_x,0,0));
	btVector3 rear_tire = quatRotate(chasis->getOrientation(), btVector3(-offset_x,0,0));
	btVector3 velocity = u*chasis->getLinearVelocity().dot(u);	//do a projection in direction we are travelling
	btScalar speed = velocity.length();

	
	/*hacking user input*/
	btScalar engine_force = 0;
	if(isForward)
	{
		engine_force = 2;
		torque += engine_torque;
	}
	
	if(isBackward)
	{
		engine_force = 2;	//for now just make it one or the other
		// torque += engine_force;
		torque = 0;
		// u *= -1;
		//f_breaking = -u * c_breaking;
		
	}
	
	//rear wheel driving
	btVector3 long_force(0,0,0);
	btVector3 forward(0,0,0);
	if(engine_force != 0)	//only call if wheels are doing something fancy
	{
		btVector3 f0 = springs[0]->getForce(torque, chasis->getLinearVelocity(), u );
		btVector3 f1 = springs[1]->getForce(torque, chasis->getLinearVelocity(), u );
		chasis->applyCentralForce(f0);
		chasis->applyCentralForce(f1);
		
		forward = f0 / 2.0 + f1 / 2.0;
		
		long_force += (f0 + f1);
	}
	
	btVector3 lat0 = springs[0]->getLateralForce(chasis->getLinearVelocity(), u );
	btVector3 lat1 = springs[1]->getLateralForce(chasis->getLinearVelocity(), u );
	
	if(lat0.length() > 0)
	{
		chasis->applyCentralForce(lat0 + lat1);
	}
		
	/*air resistance*/
	 btVector3 f_drag = -c_drag * velocity * speed;
	
	/*rolling resistance*/
	btScalar c_rolling = c_roll * c_drag;
	btVector3 f_rolling = -c_rolling * velocity;
	
	chasis->applyCentralForce( f_drag + f_rolling );
		
	/*find car acceleration*/
	long_force += (f_drag + f_rolling);
	btScalar long_scalar_force = long_force.dot(u);
	btScalar long_acceleration = long_scalar_force / mass;
	std::cout << "acceleration:" << long_acceleration << std::endl;
	
	/*weight shift*/
	btScalar L = 2*offset_x;
	btScalar c = fabs(offset_x - comx);
	btScalar b = fabs(offset_x - comx);
	btScalar W = mass * gravity;
	btScalar h = fabs(spring_bottom - comz);
	weight_front = ((c/L)*W - (h/L)*mass*long_acceleration*torque_k) * weight_shift + weight_front * (1-weight_shift) ;
	weight_rear = ((b/L)*W + (h/L)*mass*long_acceleration*torque_k) * weight_shift + weight_rear * (1-weight_shift);
	
	std::cout << "weight_front:" << weight_front << std::endl;
	std::cout << "weight_rear :" << weight_rear << std::endl;
	
	btVector3 gravity_v(0,-1.0, 0);
	std::cout << "gravity_v: " << gravity_v.x() <<","<< gravity_v.y() <<"," << gravity_v.z() << std::endl;
	chasis->applyForce(gravity_v * weight_front, front_tire);
	chasis->applyForce(gravity_v * weight_rear, rear_tire);
	
	torque /= 1.1;
	
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
