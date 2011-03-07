#include "Spring.h"
#include <iostream>
#include "Common/SettingsFactory.h"
#include <math.h>
#include "Main/MainController.h"
#include "Common/Debug.h"
#include "Physics.h"
#include "Common/Actor.h"
#include <btBulletDynamicsCommon.h>

// RenderObject Spring::wheelModel;

// #define DEBUG_RENDERER
btScalar Spring::slip_ratio_lookup(btScalar slip)	//replace with a real lookup
{
	return slip*200.0;
	LOG("slip " << slip, "slip" );
	btScalar ret = 0;
	if(slip < 3)
	{
		ret = slip * 4;
	}
	
	if(slip < 0)
	{
		ret = slip*4;
	}
	
	if(slip > 3)
	{
		ret = 2;
		if( 12 - slip/2.0 > 1)
			ret = (12-slip/2.0);
		else
			ret = 1;
	}
	
	
	
	if(ret > LoadFloat("config/jeep_springs.xml", "max_torque"))
		ret = LoadFloat("config/jeep_springs.xml", "max_torque");
	if(ret < -LoadFloat("config/jeep_springs.xml", "max_torque"))
		ret = -LoadFloat("config/jeep_springs.xml", "max_torque");
	
	return ret;
}

btVector3 Spring::getFriction(btVector3 const & linear_velocity, btVector3 const & angular_velocity) const
{
	static btScalar c = LoadFloat("config/jeep_springs.xml", "c_roll3");
	btVector3 heading = quatRotate(current_direction, btVector3(1,0,0) );
	btScalar aligned = heading.dot(linear_velocity);
	btVector3 d = c*quatRotate(current_direction, btVector3(1,0,0) * aligned );
	return d;
}

btVector3 Spring::planeProjection(btVector3 const & tire_direction) const
{
	btScalar k = tire_direction.dot(plane_normal);	//projection onto normal
	btVector3 direction = (tire_direction - k*plane_normal).normalize();	//direction on the plane
	return direction;
}

void Spring::spinTire(btVector3 const & lateral, btScalar linear_velocity)
{
	btScalar speed = 0;
	speed = -linear_velocity/100.0;
	tire_rot += speed;
	while(tire_rot > 2*3.14159)
		tire_rot -= 2*3.14159;
	while(tire_rot < -2*3.14159)
		tire_rot += 2*3.14159;
	
	btQuaternion roll = btQuaternion( btVector3(0,0,1), tire_rot);
	wheel_actor->orientation *= roll;
	
}


btVector3 Spring::getForce(btScalar torque, btVector3 const & linear_velocity, btVector3 const & tire_direction)
{	
	
	wheel_speed = torque;		//set engine on
	
	if(current_weight < 1e-6)	//off the ground
	{
		return btVector3(0,0,0);
	}
	
	btVector3 direction = planeProjection(tire_direction);
	btScalar tire_speed = direction.dot(linear_velocity);	//checks contribution to tire speed on this plane		
	btScalar weightScale = btVector3(0,1,0).dot(this->plane_normal);
	btScalar slip_ratio = (wheel_speed * wheel_radius - tire_speed) / (fabs(tire_speed) + 0.001);	//0.001 deals with speed=0
	LOG("slip_ratio:" << slip_ratio, "springs");
	// return direction * slip_ratio_lookup(slip_ratio);
	return direction*wheel_speed*weightScale;
	
	
}

btVector3 Spring::getLateralForce(btVector3 const & linear_velocity, btVector3 const & tire_direction)
{
	
	if(current_weight <1e-6)	//off the ground
	{
		return btVector3(0,0,0);
	}
	
	btVector3 direction = planeProjection(tire_direction);	//direction on the plane
	btVector3 lateral = direction.cross(plane_normal);
	if(lateral.dot(linear_velocity) > 0)	//on the same half-plane so flip it
		lateral *= -1;
	
	
	btVector3 planar_direction = planeProjection(linear_velocity);
	btScalar dir = planar_direction.dot(direction);
	
	if(dir > 1)
		dir = 1;
	if(dir<-1)
		dir = -1;
	btScalar alpha = 180 * acos(dir) / 3.14159;
	
	return alpha * lateral;
	
}

Spring::Spring(btRigidBody * const chasis, btVector3 const & from, btVector3 const & to, Physics * const physics) : 
chasis(chasis),
from(from), 
to(to), 
physics(physics), 
wheel_radius(LoadFloat("config/spring.xml", "radius")),
wheelModel("jeep_uv2.png", "blank.bmp", "models/wheel2.obj"),
current_direction(0,0,0)
{
	#ifdef DEBUG_RENDERER
	debugger = physics->dynamicsWorld.getDebugDrawer();
	#endif
	was_hit = false;
	current_weight = 0;
	wheel_speed = 0;
	
	wheel_actor = new Actor(wheelModel, btVector3(0,0,0));
	wheel_actor->orientation = btQuaternion::getIdentity();
	
	MainController::addActor(wheel_actor);
	old_x = 0;
	tire_rot = 0;
}

btScalar Spring::getWeight()
{
	return current_weight;
}

void Spring::render()
{
	if(plane_normal.length() == 0)	//in the air
		wheel_actor->pos = to + quatRotate(chasis->getOrientation(), btVector3(0,wheel_radius,0) );
	else
		wheel_actor->pos = hitPoint + quatRotate(chasis->getOrientation(), btVector3(0,wheel_radius,0) );
	
}

void Spring::tick(seconds timeStep, btVector3 const & pos, btScalar steer_angle)
{
	
	static btScalar const & gravity = fabs(LoadFloat("config/world.xml", "gravity"));
	static btScalar const & rest_fraction = LoadFloat("config/spring.xml", "rest");
	static btScalar const & k= LoadFloat("config/spring.xml", "k");
	static btScalar const & k2 = LoadFloat("config/spring.xml", "c");
	static btScalar	const & mass = LoadFloat("config/jeep_springs.xml", "mass");
	static btScalar	const & weight = mass * gravity / 4.0;
	btScalar c = k2*sqrt(k/mass);

	btQuaternion steer = btQuaternion( quatRotate(chasis->getOrientation(), btVector3(0,1,0) ), steer_angle);
	current_direction = chasis->getOrientation() * steer;
	wheel_actor->orientation = current_direction;


	btVector3 rest = rest_fraction*(to - from);
	btVector3 spring_unit = (to - from).normalize();
	btCollisionWorld::ClosestRayResultCallback result(from, to);
	physics->dynamicsWorld.rayTest( from, to, result);	

	btVector3 physical_spring = (result.m_hitPointWorld - from);	//the spring as it is compressed
	btScalar physical_length = physical_spring.length();
	
	if(result.hasHit() &&  physical_length <= ( (to-from) ).length() )
	{		
		plane_normal = result.m_hitNormalWorld;
		this->hitPoint = result.m_hitPointWorld;
		btScalar x = rest.length() - physical_length;

		btScalar spring_v = (x - old_x) * timeStep;		
		btVector3 projection = -spring_unit;
		
		btScalar force = k*x + c*spring_v + weight;
		btVector3 fromPoint = (from - this->hitPoint).normalized();
		
		#ifdef DEBUG_RENDERER		
		debugger->drawLine(pos, pos + fromPoint*10, btVector3(0,0,0));		
		LOG("DEBUG_RENDERER is ON", "springs");
		#endif
		
		btScalar angle_scale = projection.dot( this->plane_normal );
		force *= angle_scale;
		if(force > 0)
		{
			chasis->applyForce( projection * force , from - pos);
			current_weight = force;
		}
		chasis->activate();
		old_x = x;		
	}else
	{
		plane_normal = btVector3(0,0,0);
		
		#ifdef DEBUG_RENDERER
		debugger->drawLine(from, to, btVector3(0,0,0));
		LOG("DEBUG_RENDERER is ON", "springs");
		#endif
		old_x = (to-from).length();
		current_weight = 0;
	}
}