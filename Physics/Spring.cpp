#include "Spring.h"
#include <iostream>
#include "Common/SettingsFactory.h"
#include <math.h>
#include "Main/MainController.h"
#include "Common/Debug.h"

// RenderObject Spring::wheelModel;

// #define DEBUG_RENDERER
btScalar Spring::slip_ratio_lookup(btScalar slip)	//replace with a real lookup
{
	if(slip < 3)
	{
		return slip * 4;
	}
	
	if(slip < 0)
	{
		return slip*4;
	}
	
	if(slip > 3)
	{
		if( 12 - slip/2.0 > 1)
			return (12-slip/2.0);
		else
			return 1;
	}
}

btVector3 const & Spring::getFriction(btVector3 const & linear_velocity, btVector3 const & angular_velocity) const
{
	static btScalar c = LoadFloat("config/jeep_springs.xml", "c_roll3");
	btVector3 heading = quatRotate(current_direction, btVector3(1,0,0) );
	btScalar aligned = heading.dot(linear_velocity);
	btVector3 d = c*quatRotate(current_direction, btVector3(1,0,0) * aligned );
	LOG(d, "jeep");
	return d;
}

btVector3 const & Spring::planeProjection(btVector3 const & tire_direction) const
{
	btScalar k = tire_direction.dot(plane_normal);	//projection onto normal
	btVector3 direction = (tire_direction - k*plane_normal).normalize();	//direction on the plane
	return direction;
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
	btScalar slip_ratio = (wheel_speed * wheel_radius - tire_speed) / (fabs(tire_speed) + 0.001);	//0.001 deals with speed=0
	LOG("slip_ratio:" << slip_ratio, "springs");
	return direction * slip_ratio_lookup(slip_ratio) * current_weight;
	
	
}

btVector3 Spring::getLateralForce(btVector3 const & linear_velocity, btVector3 const & tire_direction)
{
	
	if(current_weight <1e-6 || linear_velocity.length2() < 1e-6)	//off the ground
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
wheelModel("blank.bmp", "wheel_final.obj"),
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
}

btScalar Spring::getWeight()
{
	return current_weight;
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
	LOG("to:" << to, "springs");
	LOG("from:" << to, "springs");

	if(result.hasHit() )
	{		
		
		plane_normal = result.m_hitNormalWorld;

		btVector3 physical_spring = (result.m_hitPointWorld - from);	//the spring as it is compressed
		btScalar physical_length = physical_spring.length();

		btScalar x = rest.length() - physical_length;

		btScalar spring_v = (x - old_x) * timeStep;
		
		if(!was_hit)
			spring_v = 0;
				
		#ifdef DEBUG_RENDERER		
		debugger->drawLine(from, from + physical_spring, btVector3(0,0,0));		
		LOG("DEBUG_RENDERER is ON", "springs");
		#endif
		wheel_actor->pos = result.m_hitPointWorld + btVector3(0,wheel_radius,0);
		
		btVector3 projection = -spring_unit;
		
		btScalar force = k*x + c*spring_v + weight;
		btScalar angle_scale = projection.dot(result.m_hitNormalWorld);
		force *= angle_scale;
		if(force > 0)
		{
			chasis->applyForce( projection * force , from - pos);
			LOG("spring_force:" << force, "springs");
			current_weight = force;
		}
		chasis->activate();
		old_x = x;		
		was_hit = true;	
	}else
	{
		plane_normal = btVector3(0,0,0);
		wheel_actor->pos = to + btVector3(0,wheel_radius,0);
		#ifdef DEBUG_RENDERER
		debugger->drawLine(from, to, btVector3(0,0,0));
		LOG("DEBUG_RENDERER is ON", "springs");
		#endif
		was_hit = false;
		current_weight = 0;
	}
}