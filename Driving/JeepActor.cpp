#include "JeepActor.h"
#include <iostream>
#include "Common/SettingsFactory.h"
#include "Common/Debug.h"

#define DEBUG_DRAW

float JeepActor::gravity = 1;
float JeepActor::mass = 1;

JeepActor::JeepActor(PhysObject const & physObj, RenderObject const & renderObj, Physics * const physics, IInput const * input, btVector3 const & pos, btVector3 const & vel) : 
Actor(physObj, renderObj, pos, vel),
physics(physics), offset_x(LoadFloat("config/jeep_springs.xml", "offset_x")),
offset_z(LoadFloat("config/jeep_springs.xml", "offset_z")),
spring_top(LoadFloat("config/jeep_springs.xml", "spring_top")), 
spring_bottom(LoadFloat("config/jeep_springs.xml", "spring_bottom")),
input(input),
u(1,0,0),
up_axis(0,1,0),
lateral(0,0,1),
c_drag( LoadFloat("config/jeep_springs.xml", "c_drag") ),
c_roll( LoadFloat("config/jeep_springs.xml", "c_roll") ),
c_roll2( LoadFloat("config/jeep_springs.xml", "c_roll2") ),
max_rotate( LoadFloat("config/jeep_springs.xml", "max_rotate") ),
turn_time( LoadFloat("config/jeep_springs.xml", "turn_time") )
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
	// physics->dynamicsWorld.setInternalTickCallback(myTickCallback, static_cast<void *>(this), true );	//setup spring callback
	chasis->setGravity(btVector3(0,0,0));	//we do it manually
	// chasis->applyImpulse(btVector3(150, 30.5, 0), btVector3(0.1,0,0));

	for(int i=0; i<4; i++)
		springs.push_back(new Spring(chasis, from[i], to[i], physics) );	
		
	mass = LoadFloat("config/jeep_springs.xml", "mass");	
	gravity = LoadFloat("config/world.xml", "gravity");

	delta = 0;
}

void JeepActor::render()
{
	for(int i=0; i<4; i++)
		springs[i]->render();
}

void JeepActor::myTickCallback(btDynamicsWorld *world, btScalar timeStep)
{
	JeepActor * jeep = static_cast<JeepActor *>(world->getWorldUserInfo());
		LOG("pos " << jeep->pos, "temp");
	jeep->tick(timeStep);
}

/*rolling resistance*/
btVector3 JeepActor::long_friction()
{
	btScalar c_rolling = c_roll * c_drag;
	btVector3 f_rolling = -c_rolling * this->long_velocity;
	return f_rolling;
	
}

/*lateral friction*/
btVector3 JeepActor::lateral_friction(btScalar delta)
{
	/*btVector3 total(0,0,0);
	for(int i=0; i<4; i++)
	{
		total += springs[i]->getLateralForce( velocity, u) * c_roll2;
	}
	
	return total;*/
	
	//no drifting but good enough for now
	if(!onGround)
		return btVector3(0,0,0);
		
	return lateral*(-velocity.dot(lateral) * c_roll2);// * pow(1-fabs(delta), 3);
}

/*air resistance*/
btVector3 JeepActor::air_resistance()
{
	return c_drag * (-velocity) * speed;
}

btVector3 JeepActor::update_tires()
{
	btVector3 f0 = springs[0]->getForce(this->engine.torque, this->velocity , this->u );
	btVector3 f1 = springs[1]->getForce(this->engine.torque, this->velocity, this->u );
		
	return f0 / 2.0 + f1 / 2.0;
}


void JeepActor::isOnGround()
{
	for(int i=0; i<4; i++)
	{
		if(this->springs[i]->plane_normal.length() > 0)
		{
			this->onGround = true;
			return;
		}
	}
	this->onGround = false;
}


/*weight shift
void JeepActor::weight_shift()
{
/*find car acceleration*/
/*
//calculate state of the jeep
btVector3 front_tire = quatRotate(chasis->getOrientation(), btVector3(offset_x,0,0));
btVector3 rear_tire = quatRotate(chasis->getOrientation(), btVector3(-offset_x,0,0));

btScalar long_acceleration = long_scalar_force / mass;

//weight shift
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

}*/


void JeepActor::tick(seconds timeStep)
{
	
	/*get steering info*/
	delta += (-input->XAxis * max_rotate - delta) / turn_time;
	for(int i=0; i<4; i++)
	{
		if(i == 2 || i == 3)
			springs[i]->tick(timeStep, pos, delta);	/*apply springs*/
		else
			springs[i]->tick(timeStep, pos, 0);	/*apply springs*/
	}
		
	/*various velocities*/
	this->velocity = chasis->getLinearVelocity();
	speed = this->velocity.length();
	this->long_speed = this->velocity.dot(u);
	this->long_velocity = u*this->long_speed;
	
	isOnGround();	//update the onGround status
	
	//accumulate forces acting on jeep
	btVector3 central_forces = btVector3(0,0,0);
	
	if(input->AcceleratePressed)
	{
		engine.accelerate();
		central_forces += update_tires();
	}

	if(input->BrakePressed)
	{
		engine.decelerate();
		central_forces += update_tires();
	}
	
	//LOG("inputs gas break steer" << input->AcceleratePressed <<" "<< input->BrakePressed <<" "<< input->XAxis, "temp");
	
	//calculate environment forces on jeep	
	//gravity
	central_forces += btVector3(0, mass*gravity, 0);
	
	//friction
	central_forces += long_friction();
	central_forces += lateral_friction(delta);
	
		
	//air resistance
	central_forces += air_resistance();
	
	
	chasis->applyCentralForce(central_forces);	
			
	
	/*debugging jump*/
	if(input ->EBrakePressed)
	{
		chasis->applyImpulse( btVector3(0,40,0), btVector3(0,0,0));
	}


	btVector3 plane_up = btVector3(0,1,0);	//in air so just normal
	float count = 0;
	for(int i=0; i<4; i++)
	{
		if(springs[i]->plane_normal.length() >= 1)
		{
			plane_up += springs[i]->plane_normal;
			count += 1;
		}
	}

	if(count > 0)
	{
		plane_up *= (1/count);	//average of the plane normals
	}

	
	//angular friction
	btScalar angular = chasis->getAngularVelocity().length();
	chasis->applyTorque( -chasis->getAngularVelocity() * LoadFloat("config/jeep_springs.xml", "rotate_friction"));
		
	//turning	
	btScalar turning_weight = (springs[1]->getWeight() + springs[3]->getWeight() ) /2.0;
	
	if(turning_weight > 1e-6) {
		btVector3 angular_vel = chasis->getAngularVelocity();
		btScalar turn_factor = this->long_speed + LoadFloat("config/jeep_springs.xml", "turn_boost")/(0.1+chasis->getAngularVelocity().length() );
		
		
		chasis->applyTorque( LoadFloat("config/jeep_springs.xml", "turn_k") * delta * plane_up * turn_factor);
		
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
	
	/*update frame of jeep*/
	u = quatRotate(rot, btVector3(1,0,0) );
	up_axis = quatRotate(rot, btVector3(0,1,0) );
	lateral = quatRotate(rot, btVector3(0,0,1) );
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
