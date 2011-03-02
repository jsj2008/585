#include "JeepActor.h"
#include <iostream>
#include "Common/SettingsFactory.h"
#include "Common/Debug.h"

// #define DEBUG_DRAW

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
	physics->dynamicsWorld.setInternalTickCallback(myTickCallback, static_cast<void *>(this), true );	//setup spring callback
	chasis->setGravity(btVector3(0,0,0));	//we do it manually
	// chasis->applyImpulse(btVector3(150, 30.5, 0), btVector3(0.1,0,0));

	for(int i=0; i<4; i++)
		springs.push_back(new Spring(chasis, from[i], to[i], physics) );
	
	#ifdef DEBUG_RENDERER
	debugger = physics->dynamicsWorld.getDebugDrawer();
	#endif
	
}

void JeepActor::render()
{
	for(int i=0; i<4; i++)
		springs[i]->render();
}

void JeepActor::myTickCallback(btDynamicsWorld *world, btScalar timeStep)
{
	JeepActor * jeep = static_cast<JeepActor *>(world->getWorldUserInfo());
	jeep->tick(timeStep);
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
		if(torque < 0)
			torque = 0;
		engine_force = 2;
		torque += engine_torque;
	}
	
	btScalar XAxis = -1*input->XAxis;

	if(input->BrakePressed)
	{
		if(torque > 0)
			torque = 0;
		engine_force = 2;//torque;	//for now just make it one or the other
		// torque += engine_force;
		torque -= engine_torque;
		//XAxis *= -1;
		// u *= -1;
		//f_breaking = -u * c_breaking;
		
	}
	
	if(torque > LoadFloat("config/jeep_springs.xml", "max_torque"))
	{
		torque = LoadFloat("config/jeep_springs.xml", "max_torque");
	}
	
	if(torque < -LoadFloat("config/jeep_springs.xml", "max_torque"))
	{
		torque = -LoadFloat("config/jeep_springs.xml", "max_torque");
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

	btVector3 velocity = chasis->getLinearVelocity();
	btVector3 lateral = quatRotate(chasis->getOrientation(), btVector3(0,0,1));

	btScalar long_speed = velocity.dot(u);
	btVector3 long_velocity = u*long_speed;
	
	
	//rear wheel driving
	btVector3 long_force(0,0,0);
	btVector3 forward(0,0,0);
	if(engine_force > 0)	//only call if wheels are doing something fancy
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
	 btVector3 f_drag = -c_drag * velocity * fabs(velocity.length() );
	
	/*rolling resistance*/
	btScalar c_rolling = c_roll * c_drag;
	btVector3 f_rolling = -c_rolling * long_velocity;
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
	
	chasis->applyCentralForce(btVector3(0,gravity*mass,0));
	chasis->applyForce(btVector3(0,-1.0,0) * weight_front, front_tire);
	chasis->applyForce(btVector3(0,-1.0,0) * weight_rear, rear_tire);
	
	
	torque /= 1.1;
	
			
	btVector3 up_axis = quatRotate(chasis->getOrientation(), btVector3(0,1,0) );
	
	/*debugging jump*/
	if(input ->EBrakePressed)
	{
		chasis->applyImpulse( btVector3(0,40,0), btVector3(0,0,0));
	}
	
	
	btVector3 real_up = btVector3(0,0,0);
	float count = 0;
	for(int i=0; i<4; i++)
	{
		if(springs[i]->plane_normal.length() >= 1)
		{
			real_up += springs[i]->plane_normal;
			count += 1;
		}
	}
	
	if(count > 0)
	{
		real_up *= (1/count);
	}else
	{
		real_up = btVector3(0,1,0);
	}

	

	/*set up correction frame*/
	btScalar k = u.dot(real_up);	//projection onto real_up
	btVector3 real_forward = u - k*real_up;
	btVector3 real_lateral = real_forward.cross(real_up);
	
	
	#ifdef DEBUG_DRAW
	// physics->dynamicsWorld.getDebugDrawer()->drawLine(pos, pos + real_forward*10, btVector3(0,0,0));		
	// 	physics->dynamicsWorld.getDebugDrawer()->drawLine(pos, pos + up_axis*10, btVector3(255,0,0));		
		physics->dynamicsWorld.getDebugDrawer()->drawLine(pos, pos + real_up*10, btVector3(0,0,0));		
		physics->dynamicsWorld.getDebugDrawer()->drawLine(pos, pos + real_lateral*10, btVector3(0,0,0));		
	// 	physics->dynamicsWorld.getDebugDrawer()->drawLine(pos, pos + lateral*10, btVector3(255,0,0));		
	 	#endif
	
	/*forward tilt*/
	// real_up = btVector3(0,1,0);
	static btScalar lastX = 0;
	btScalar x = -u.dot(real_up);
	if( x >  0.1)	//sinking
	{
		// chasis->applyTorque(real_lateral *LoadFloat("config/jeep_springs.xml", "auto_correct")  );
		btTransform current = chasis->getWorldTransform();
		//btTransform atOrigin = chasis->.translate(-pos);
		btQuaternion toRotate = current.getRotation();
		toRotate *= btQuaternion(real_lateral, x);
		current.setRotation(toRotate);
		//btTransform final = current.translate(pos);
		
		//chasis->setWorldTransform(current);
		
		btVector3 oldAngular = chasis->getAngularVelocity();
		// chasis->setAngularVelocity(oldAngular * 0.98);
		
		// btVector3 want = velocity;
		
		// chasis->applyCentralForce(real_up * LoadFloat("config/jeep_springs.xml", "auto_correct") );
		#ifdef DEBUG_DRAW
		physics->dynamicsWorld.getDebugDrawer()->drawLine(pos, pos + up_axis*10, btVector3(255,0,0));		
		#endif
	}
	lastX = x;
	
	/*lateral tilt*/
	x = lateral.dot(real_up);
	if( fabs(x) > 1.1)
	{
		// chasis->applyTorque(real_forward * x * LoadFloat("config/jeep_springs.xml", "auto_correct")  );
		
		#ifdef DEBUG_DRAW
		// physics->dynamicsWorld.getDebugDrawer()->drawLine(pos, pos + lateral*10, btVector3(255,0,0));		
		#endif
	}

	
	//angular friction
	//btScalar angular = chasis->getAngularVelocity().dot(real_up);
	//LOG("angular " << angular, "jeep");
	//chasis->applyTorque( -angular * real_up * LoadFloat("config/jeep_springs.xml", "rotate_friction"));
	btScalar angular = chasis->getAngularVelocity().length();
	// chasis->applyTorque( -angular * real_up * LoadFloat("config/jeep_springs.xml", "rotate_friction"));
	chasis->applyTorque( -chasis->getAngularVelocity() * LoadFloat("config/jeep_springs.xml", "rotate_friction"));
		
	//turning
	
	btScalar turning_weight = (springs[1]->getWeight() + springs[3]->getWeight() ) /2.0;
	
	if(turning_weight > 1e-6) {
		chasis->applyTorque( LoadFloat("config/jeep_springs.xml", "turn_k") * delta * real_up * long_speed);
		
		btScalar delta_sign = delta < 0 ? -1 : +1;
		chasis->applyCentralForce( lateral*delta_sign *
			chasis->getAngularVelocity().length() * LoadFloat("config/jeep_springs.xml", "centrifugal"));
	}
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
