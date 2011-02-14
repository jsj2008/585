#include "JeepActor.h"
#include <iostream>
#include "Common/SettingsFactory.h"

JeepActor::JeepActor(PhysObject const & physObj, RenderObject const & renderObj, Physics * const physics,  Input const * input, btVector3 const & pos, btVector3 const & vel) : 
Actor(physObj, renderObj, pos, vel),
physics(physics), offset_x(LoadFloat("config/jeep_springs.xml", "offset_x")),
offset_z(LoadFloat("config/jeep_springs.xml", "offset_z")),
spring_top(LoadFloat("config/jeep_springs.xml", "spring_top")), 
spring_bottom(LoadFloat("config/jeep_springs.xml", "spring_bottom")),
mass(LoadFloat("config/jeep_springs.xml", "mass") ),
input(input)
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
	chasis->setGravity(btVector3(0,0,0));	//we do it manually
	// chasis->applyImpulse(btVector3(150, 30.5, 0), btVector3(0.1,0,0));

	for(int i=0; i<4; i++)
		springs.push_back(new Spring(chasis, from[i], to[i], physics) );
}

void JeepActor::tick(seconds timeStep)
{
	static float torque = 0;
	static float const & torque_k = LoadFloat("config/jeep_springs.xml", "torque_k");
	static float const & comx= LoadFloat("config/jeep_springs.xml", "comx");
	static float const & comz = LoadFloat("config/jeep_springs.xml", "comz");
	static float const & c_drag = LoadFloat("config/jeep_springs.xml", "c_drag");
	static float const & c_roll = LoadFloat("config/jeep_springs.xml", "c_roll");
	static float const & breaking = LoadFloat("config/jeep_springs.xml", "breaking");
	static float const & engine_torque = LoadFloat("config/jeep_springs.xml", "engine_torque");
	static float const & weight_shift = LoadFloat("config/jeep_springs.xml", "weight_shift");
	static float const & max_rotate = LoadFloat("config/jeep_springs.xml", "max_rotate");
	static float const & c_roll2 = LoadFloat("config/jeep_springs.xml", "c_roll2");
	static btScalar	const & gravity = LoadFloat("config/world.xml", "gravity");
	static btScalar weight_front = 0;
	static btScalar weight_rear = 0;
	static btScalar turn_time = LoadFloat("config/jeep_springs.xml", "turn_time");
	/*get steering info*/
	
	btScalar engine_force = 0;
	if(input->AcceleratePressed)
	{
		engine_force = 2;
		torque += engine_torque;
	}
	
	btScalar XAxis = -1*input->XAxis;

	if(input->BrakePressed)
	{
		engine_force = 2;	//for now just make it one or the other
		// torque += engine_force;
		torque -= engine_torque;
		XAxis *= -1;
		// u *= -1;
		//f_breaking = -u * c_breaking;
		
	}

	
	static btScalar delta = 0;
	delta += (XAxis * max_rotate - delta) / turn_time;
	for(int i=0; i<4; i++)
	{
		// std::cout << i << std::endl;
		if(i == 2 || i == 3)
			springs[i]->tick(timeStep, pos, delta);	/*apply springs*/
		else
			springs[i]->tick(timeStep, pos, 0);	/*apply springs*/
	}
	
	/*calculate constants*/
	btVector3 u = quatRotate(chasis->getOrientation(), btVector3(1,0,0) );
	btVector3 front_tire = quatRotate(chasis->getOrientation(), btVector3(offset_x,0,0));
	btVector3 rear_tire = quatRotate(chasis->getOrientation(), btVector3(-offset_x,0,0));
	btVector3 velocity = u*chasis->getLinearVelocity().dot(u);	//do a projection in direction we are travelling
	btScalar speed = velocity.length();

	
	
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
		chasis->applyCentralForce((lat0 + lat1) * c_roll2);
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
	//std::cout << "acceleration:" << long_acceleration << std::endl;
	
	/*weight shift*/
	btScalar L = 2*offset_x;
	btScalar c = fabs(offset_x - comx);
	btScalar b = fabs(offset_x - comx);
	btScalar W = mass * fabs(gravity);
	btScalar h = fabs(spring_bottom - comz);
	weight_front = ((c/L)*W - (h/L)*mass*long_acceleration*torque_k) * weight_shift + weight_front * (1-weight_shift) ;
	weight_rear = ((b/L)*W + (h/L)*mass*long_acceleration*torque_k) * weight_shift + weight_rear * (1-weight_shift);
	
	// std::cout << "weight_front:" << weight_front << std::endl;
	// std::cout << "weight_rear :" << weight_rear << std::endl;
	
	chasis->applyCentralForce(btVector3(0,gravity*mass,0));
	//std::cout << "gravity_v: " << gravity_v.x() <<","<< gravity_v.y() <<"," << gravity_v.z() << std::endl;
	chasis->applyForce(btVector3(0,-1.0,0) * weight_front, front_tire);
	chasis->applyForce(btVector3(0,-1.0,0) * weight_rear, rear_tire);
	
	torque /= 1.1;
	
	/*moving sideways*/
	btScalar omega = 0;
	if(delta != 0)	//if actually turning
	{
		btScalar R = L/sin(delta);
		omega = speed / R;
	}
		//chasis->applyForce( btVector3(0,0,omega*100), front_tire);
		// chasis->applyTorque( btVector3(0,omega*3,0));
	chasis->setAngularVelocity(btVector3(0,omega*0.75,0));
	
}

JeepActor::~JeepActor()
{
	for(int i=0; i<4; i++)
	{
		delete springs[i];
	}
}

void JeepActor::setOrientation(btQuaternion const & rot)
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

void JeepActor::setPosition(btVector3 const & pos)
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
