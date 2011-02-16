#include "Spring.h"
#include <iostream>
#include "Common/SettingsFactory.h"
#include <math.h>
#include "Main/MainController.h"

// RenderObject Spring::wheelModel;


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

btVector3 Spring::getForce(btScalar torque, btVector3 const & linear_velocity, btVector3 const & tire_direction)
{
	wheel_speed = torque;		//set engine on
	
	if(current_weight == 0)	//off the ground
	{
		return btVector3(0,0,0);
	}
	
	btScalar k = tire_direction.dot(plane_normal);	//projection onto normal
	btVector3 direction = (tire_direction - k*plane_normal).normalize();	//direction on the plane
	btScalar tire_speed = direction.dot(linear_velocity);	//checks contribution to tire speed on this plane		
	btScalar slip_ratio = (wheel_speed * wheel_radius - tire_speed) / (fabs(tire_speed) + 0.001);	//0.001 deals with speed=0
	//std::cout << "slip_ratio:" << slip_ratio << std::endl;	
	
	return direction * slip_ratio_lookup(slip_ratio) * current_weight;
	
	
}

btVector3 Spring::getLateralForce(btVector3 const & linear_velocity, btVector3 const & tire_direction)
{
	
	if(current_weight == 0 || linear_velocity.length() == 0)	//off the ground
	{
		return btVector3(0,0,0);
	}
	
	btScalar k = tire_direction.dot(plane_normal);	//projection onto normal
	btVector3 direction = (tire_direction - k*plane_normal).normalize();	//direction on the plane
	//std::cout << "direction:" << direction.x() << "," << direction.y() << "," << direction.z() << std::endl;
	btVector3 lateral = direction.cross(plane_normal);
	if(lateral.dot(linear_velocity) > 0)	//on the same half-plane so flip it
		lateral *= -1;

	//std::cout << "lateral:" << lateral.x() << "," << lateral.y() << "," << lateral.z() << std::endl;		
	
	btScalar k2 = linear_velocity.dot(plane_normal);	//projection onto normal

	btVector3 planar_direction = (linear_velocity - k2*plane_normal).normalize();	//linear velocity on plane
	//std::cout << "linear:" << planar_direction.x() << "," << planar_direction.y() << "," << planar_direction.z() << std::endl;			
	btScalar alpha = 180 * acos(planar_direction.dot(direction) ) / 3.14159;
	//std::cout <<  "alpha:" << alpha << std::endl;
	
	return alpha * lateral;
	
}


Spring::Spring(btRigidBody * const chasis, btVector3 const & from, btVector3 const & to, Physics * const physics) : 
chasis(chasis), from(from), to(to), physics(physics), wheel_radius(LoadFloat("config/spring.xml", "radius")),
wheelModel("blank.bmp", "wheel_final.obj")
{
	// debugger = physics->dynamicsWorld.getDebugDrawer();
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
	wheel_actor->orientation = chasis->getOrientation() * steer;


	btVector3 rest = rest_fraction*(to - from);
	btVector3 spring_unit = (to - from).normalize();

	btCollisionWorld::ClosestRayResultCallback result(from, to);	
	physics->dynamicsWorld.rayTest( from, to, result);
	// std::cout << to.x() << "," << to.y() << "," << to.z() << std::endl;
	// std::cout << from.x() << "," << from.y() << "," << from.z() << std::endl;
	if(result.hasHit() )
	{		
		// std::cout << "hit: " << timeStep * 1000.0 << std::endl;
		
		plane_normal = result.m_hitNormalWorld;

		btVector3 physical_spring = (result.m_hitPointWorld - from);	//the spring as it is compressed
		btScalar physical_length = physical_spring.length();

		btScalar x = rest.length() - physical_length;



		btScalar spring_v = (x - old_x) * timeStep;
		
		if(!was_hit)
			spring_v = 0;
				
		// debugger->drawLine(from, from + physical_spring, btVector3(0,255,0));		
		// debugger->drawLine(result.m_hitPointWorld,result.m_hitPointWorld + btVector3(10,0,0) ,btVector3(255,255,0));
		// debugger->drawSphere(result.m_hitPointWorld - btVector3(0,0.25,0), 0.5, btVector3(255,0,0));
		wheel_actor->pos = result.m_hitPointWorld - btVector3(0,0.25,0);
		
		btVector3 projection = -spring_unit;//unit * unit.dot(spring_normal);
		
		btScalar force = k*x + c*spring_v + weight;
		btScalar angle_scale = projection.dot(result.m_hitNormalWorld);
		force *= angle_scale;
		if(force > 0)
		{
			chasis->applyForce( projection * force , from - pos);
			// std::cout << "spring_force:" << force << std::endl;
			current_weight = force;
		}
		chasis->activate();
		old_x = x;		
		was_hit = true;	
	}else
	{
		plane_normal = btVector3(0,0,0);
		// debugger->drawSphere(to - btVector3(0,0.25,0), 0.5, btVector3(255,0,0));
		wheel_actor->pos = to - btVector3(0,0.25,0);
		// debugger->drawLine(from, to, btVector3(0,255,0));
		was_hit = false;
		current_weight = 0;
	}
}