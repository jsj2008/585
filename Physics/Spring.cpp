#include "Spring.h"
#include <iostream>

Spring::Spring(btRigidBody * const chasis, btVector3 const & from, btVector3 const & to, Physics * const physics) : 
chasis(chasis), from(from), to(to), physics(physics){
	debugger = physics->dynamicsWorld.getDebugDrawer();
	was_hit = false;
	delta_t = 0;
	old_x = 0;
}

void Spring::tick(seconds timeStep)
{
	static btScalar k=100.0;
	static btScalar mass = 10.0;
	static btScalar c = 25*sqrt(k/mass);		

	btVector3 rest = 1*(to - from)/4.0;
	btVector3 spring_unit = (to - from).normalize();
	//debugger->drawLine(from,from + rest ,btVector3(255,0,255));
	//debugger->drawLine(from,to ,btVector3(255,0,0));

	btCollisionWorld::ClosestRayResultCallback result(from, to);	
	physics->dynamicsWorld.rayTest( from, to, result);
	
	delta_t += timeStep;	//add time for spring_velocity
	
	
	if(result.hasHit() )
	{		

		btVector3 physical_spring = (result.m_hitPointWorld - from);	//the spring as it is compressed
		btScalar physical_length = physical_spring.length();

		btScalar x = rest.length() - physical_length;

		debugger->drawLine(from, from - spring_unit * x, btVector3(0,255,125));			
		debugger->drawLine(from, from + physical_spring, btVector3(0,255,0));

		btScalar spring_v = (x - old_x) / delta_t;
		
		if(!was_hit)
			spring_v = 0;
				
		std::cout << "x:" << x << std::endl;
		std::cout << "rest_length:" << rest.length() << std::endl;
		std::cout << "physic_length:" << physical_length << std::endl;
		std::cout << "spring_vel:" << spring_v << std::endl;
		std::cout << "delta_t:" << delta_t << std::endl;

		//btVector3 unit = result.m_hitNormalWorld;//to - from;	//length is full spring length
		debugger->drawLine(result.m_hitPointWorld,result.m_hitPointWorld + btVector3(10,0,0) ,btVector3(255,255,0));
		//unit.normalize();
		
		btVector3 projection = -spring_unit;//unit * unit.dot(spring_normal);
		std::cout << "projection" << projection.y() << std::endl;
		
		btScalar force = k*x + c*spring_v + 25;
		debugger->drawLine(from,from - c*spring_v*btVector3(1,0,0) ,btVector3(125,125,125));
		btScalar angle_scale = projection.dot(result.m_hitNormalWorld);
		force *= angle_scale;
		if(force > 0)
		{
			chasis->applyForce( projection * force , from);
			debugger->drawLine(from,from + projection*(force - 25)/4.0 ,btVector3(125,125,125));
		}
		chasis->activate();
		//chasis->applyForce(-unit * k*x, from);
		//chasis->applyForce(-unit * 100, from);
		//std::cout << "kx" << k*x << std::endl;
		//std::cout << "hit" << std::endl;
		std::cout << "force:" << force << std::endl;
		old_x = x;
		
		was_hit = true;
			
	}else
	{
		was_hit = false;
		std::cout << "no hit" << std::endl;
		old_x = 0;
		delta_t = 0;
	}
	std::cout << std::endl;
}