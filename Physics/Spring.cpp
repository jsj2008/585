#include "Spring.h"
#include <iostream>
#include "Common/SettingsFactory.h"

btScalar Spring::slip_ratio_lookup(btScalar slip)	//replace with a real lookup
{
	if(slip > -5 && slip < 5)
	{
		return slip * 20;
	}
	
	if(slip > 5)
		return 70;
	if(slip < -5)
		return -70;
}

btVector3 Spring::getForce(btScalar torque, btVector3 const & linear_velocity, btVector3 const & tire_direction)
{
	wheel_speed += torque;		//set engine on
	
	if(current_weight == 0)	//off the ground
	{
		return btVector3(0,0,0);
	}
	
	btScalar k = tire_direction.dot(plane_normal);	//projection onto normal
	std::cout << "wheel:" << wheel_speed << std::endl;
	std::cout << "k: " << k << std::endl;
	btVector3 direction = (tire_direction - k*plane_normal).normalize();	//direction on the plane
	std::cout << "direction: " << direction.x() << "," << direction.y() << "," << direction.z() << std::endl;
	std::cout << "linear_velocity: " << linear_velocity.x() << "," << linear_velocity.y() << "," << linear_velocity.z() << std::endl;
	btScalar tire_speed = direction.dot(linear_velocity);	//checks contribution to tire speed on this plane
	std::cout << "tire_speed:" << tire_speed << std::endl;
		
	btScalar slip_ratio = (wheel_speed * wheel_radius - tire_speed) / (fabs(tire_speed) + 0.001);	//0.001 deals with speed=0
	std::cout << "slip_ratio:" << slip_ratio << std::endl;
	
	//integrate tire_speed into wheel_speed
	wheel_speed -= (wheel_speed * wheel_radius - tire_speed) / 10.0;	//make wheel_speed (non accelerating rolling speed)
	
	return direction * slip_ratio_lookup(slip_ratio) * current_weight;
	
	
}

Spring::Spring(btRigidBody * const chasis, btVector3 const & from, btVector3 const & to, Physics * const physics) : 
chasis(chasis), from(from), to(to), physics(physics), wheel_radius(LoadFloat("config/spring.xml", "radius")){
	debugger = physics->dynamicsWorld.getDebugDrawer();
	was_hit = false;
	current_weight = 0;
	wheel_speed = 0;
}

btScalar Spring::getWeight()
{
	return current_weight;
}

void Spring::tick(seconds timeStep, btVector3 const & pos)
{
	static btScalar const & k= LoadFloat("config/spring.xml", "k");
	static btScalar const & k2 = LoadFloat("config/spring.xml", "c");
	static btScalar	mass = 10.0;
	btScalar c = k2*sqrt(k/mass);

	static btScalar const & rest_fraction = LoadFloat("config/spring.xml", "rest");
	btVector3 rest = rest_fraction*(to - from);
	btVector3 spring_unit = (to - from).normalize();

	btCollisionWorld::ClosestRayResultCallback result(from, to);	
	physics->dynamicsWorld.rayTest( from, to, result);
	
	if(result.hasHit() )
	{		
		plane_normal = result.m_hitNormalWorld;

		btVector3 physical_spring = (result.m_hitPointWorld - from);	//the spring as it is compressed
		btScalar physical_length = physical_spring.length();

		btScalar x = rest.length() - physical_length;

		debugger->drawLine(from, from + physical_spring, btVector3(0,255,0));

		btScalar spring_v = (x - old_x) * timeStep;
		
		if(!was_hit)
			spring_v = 0;
				

		debugger->drawLine(result.m_hitPointWorld,result.m_hitPointWorld + btVector3(10,0,0) ,btVector3(255,255,0));
		debugger->drawSphere(result.m_hitPointWorld - btVector3(0,0.25,0), 0.5, btVector3(255,0,0));
		
		btVector3 projection = -spring_unit;//unit * unit.dot(spring_normal);
		
		btScalar force = k*x + c*spring_v + 25;
		btScalar angle_scale = projection.dot(result.m_hitNormalWorld);
		force *= angle_scale;
		if(force > 0)
		{
			chasis->applyForce( projection * force , from - pos);
			current_weight = force;
		}
		chasis->activate();
		old_x = x;		
		was_hit = true;
			
	}else
	{
		plane_normal = btVector3(0,0,0);
		debugger->drawSphere(to - btVector3(0,0.25,0), 0.5, btVector3(255,0,0));
		debugger->drawLine(from, to, btVector3(0,255,0));
		was_hit = false;
		current_weight = 0;
	}
}