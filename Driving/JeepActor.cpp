#include "JeepActor.h"
#include "Common/SettingsFactory.h"
#include "Common/Debug.h"
#include "Physics/Physics.h"
#include "Physics/PhysObject.h"
#include "Physics/Spring.h"
#include <LinearMath/btIDebugDraw.h>
#include <btBulletDynamicsCommon.h>
#include "UI/IInput.h"
#include "Main/MainController.h"
#include "Audio/Sound.h"

#define DEBUG_DRAW

float JeepActor::gravity = 1;
float JeepActor::mass = 1;

JeepActor::JeepActor(PhysObject const & physObj, RenderObject const * renderObj, Physics * const physics, IInput * input, btVector3 const & pos, btQuaternion const & rot) : 
Actor(physObj, renderObj, pos),
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
turn_time( LoadFloat("config/jeep_springs.xml", "turn_time") ),
audio_frame(new float [6]),
isHuman(false),
frozen(false),
isDead(false),
isSlipping(false)
{
	orientation = rot;
	
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
    chasis->setUserPointer(this);   //hit detections will know about me!
	chasis->setGravity(btVector3(0,0,0));	//we do it manually

	for(int i=0; i<4; i++)
		springs.push_back(new Spring(chasis, from[i], to[i], physics) );	
		
	mass = LoadFloat("config/jeep_springs.xml", "mass");	
	gravity = LoadFloat("config/world.xml", "gravity");

	delta = 0;
	die_time = 0;
	long_speed = 0;
	
	//setup all the sounds
    Sound * ptr = Sound::GetInstance();
    idleSource = ptr->addSource("data/audio/engine.wav");
    hitSource = ptr->addSource("data/audio/crash.wav");
    //crashSource = ptr->addSource("data/audio/crash2.wav");    	
    //crash2Source = ptr->addSource("data/audio/crash3.wav");    	
    scratchSource = ptr->addSource("data/audio/scratch.wav");    	
    // hornSource = ptr->addSource("data/audio/horn.wav");
}

void JeepActor::reset(btQuaternion const & rot, btVector3  const &  pos)
{
	btTransform tr(rot, pos);
	chasis->setWorldTransform(tr);
	chasis->setLinearVelocity(btVector3(0,0,0));
	chasis->setAngularVelocity(btVector3(0,0,0));
		
	u = btVector3(1,0,0);	//jeep is facing this way
	up_axis = btVector3(0,1,0);	//up of jeep
	lateral = btVector3(0,0,1);	//lateral of jeep
	velocity = btVector3(0,0,0);
	speed = 0;
	long_velocity = velocity;
	long_speed = 0;
	delta = false;

	onGround = false;
    frozen = true;
	
    for(int i=0; i<4; i++)
        springs[i]->reset();
}

void JeepActor::render()
{
    Sound * ptr = Sound::GetInstance();
    ptr->setSource(idleSource, pos, velocity, u);
    engine.sound(idleSource);
    
    // if(input ->EBrakePressed)
        // ptr->setAndPlaySource(hornSource, pos);        
    
	for(int i=0; i<4; i++)
		springs[i]->render();
}

/*rolling resistance*/
btVector3 JeepActor::long_friction()
{
	btScalar extra_friction = 1;//+ u.dot(btVector3(0,1,0));
	// LOG("extra " << extra_friction, "temp");
	btScalar c_rolling = c_roll * c_drag*extra_friction;
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

void JeepActor::freezeAt(btVector3 const & pos)
{
    frozenAt = pos;
    frozen = true;
    
    for(int i=0; i<4; i++)
        springs[i]->frozen = true;
}

void JeepActor::release()
{
    this->chasis->activate(true);
    frozen = false;
    for(int i=0; i<4; i++)
        springs[i]->frozen = false;
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

void JeepActor::startEngine()
{
	Sound::GetInstance()->playSource(idleSource);
}

void JeepActor::hitObject(float impulse, btVector3 const & position)
{
    if(isDead)  //don't bother making noises if dead
        return;
    LOG("HIT is human " << isHuman, "hit");
    
    if(impulse > 500)
    {
         //Sound::GetInstance()->setAndPlaySource(crash2Source, pos);
    }
    else if(impulse > 200)
    {
         Sound::GetInstance()->setAndPlaySource(hitSource, position);
    }else if(impulse > 200 && impulse <= 400)
    {
        //Sound::GetInstance()->setAndPlaySource(hitSource, pos);        
    }else if(impulse > 50 && impulse < 200)
    {
        Sound::GetInstance()->setAndPlaySource(scratchSource, position);        
    }
    
    
}


void JeepActor::tick(seconds timeStep)
{
	/*get steering info*/
    
	delta += (-input->XAxis * max_rotate - delta) / turn_time;
	bool dying = true;
	for(int i=0; i<4; i++)
	{
		if(i == 2 || i == 3)
			springs[i]->tick(timeStep, pos, delta);	/*apply springs*/
		else
			springs[i]->tick(timeStep, pos, 0);	/*apply springs*/
			
		springs[i]->spinTire(lateral, long_speed);
		dying &= (springs[i]->plane_normal.length() == 0);	//check if all tires are off ground
	}
	
	//can freeze player if we want//
	if(frozen)
	{
        return;
	}
		
	/*check if still able to drive*/
	if(dying)
	{
		die_time += timeStep;
	}else
	{
        // if(die_time > 0.7)
            // MainController::Audio()->playJump();
		die_time = 0;
	}
	
	if(die_time > 3)	//not driving for few seconds
	{
		die_time = 0;
		input->restart();
		MainController::restartJeep(this);
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
		engine.accelerate(input->YAxis, !onGround);
		central_forces += update_tires();
		
		
	}else
	

	if(input->BrakePressed)
	{
		engine.decelerate(fabs(input->YAxis), !onGround);
		central_forces += update_tires();
        
	}else
	{
		engine.step(!onGround);
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
		// MainController::restart();
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
	    
	    btScalar long_sign = long_speed < 0 ? -1 : +1;
		btScalar delta_sign = delta < 0 ? -1 : +1;
		
	    
		btScalar angular_vel = chasis->getAngularVelocity().length();
		btScalar turn_factor = this->long_speed * long_sign*fabs(LoadFloat("config/jeep_springs.xml", "turn_boost")/(0.1+angular_vel*2.5 ) );
		
		
		
        btScalar lateral_speed = fabs(velocity.dot(lateral));
        btScalar skid = 1.0;
        
		
        btScalar centri = LoadFloat("config/jeep_springs.xml", "centrifugal");
        

        chasis->applyTorque( skid*LoadFloat("config/jeep_springs.xml", "turn_k") * delta * long_sign * plane_up * turn_factor);
        
        

		chasis->applyCentralForce( lateral*delta_sign * long_sign*
			chasis->getAngularVelocity().length() * centri);
	}
}

JeepActor::~JeepActor()
{
	for(int i=0; i<4; i++)
	{
		delete springs[i];
	}
}

void JeepActor::registerAudio(Sound * audio)
{
    // audio->SetListenerValues(&pos.x(), &velocity.y(), audio_frame);
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
	audio_frame[0] = u.x();
	audio_frame[1] = u.y();
	audio_frame[2] = u.z();
	audio_frame[3] = up_axis.x();
	audio_frame[4] = up_axis.y();
	audio_frame[5] = up_axis.z();
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
	}	
}
