#include "Spring.h"
#include <iostream>

Spring::Spring(btRigidBody * const chasis, btVector3 const & from, btVector3 const & to, Physics * const physics) : 
chasis(chasis), from(from), to(to), physics(physics){
	debugger = physics->dynamicsWorld.getDebugDrawer();
}

void Spring::tick(seconds timeStep)
{
	static btScalar old_x = 0;
	static btScalar k=10.0;
	static btScalar mass = 10.0;
	static btScalar c = 0*1.5*sqrt(k/mass);		
	static btScalar delta_t = 0;
	btVector3 rest = 2*(to - from)/4.0; //1*to/4.0 + 3*from/4.0;	
	debugger->drawLine(from,from + rest ,btVector3(255,0,255));
	debugger->drawLine(from,to ,btVector3(255,0,0));

	btCollisionWorld::ClosestRayResultCallback result(from, to);	
	physics->dynamicsWorld.rayTest( from, to, result);
	if(result.hasHit() )
	{		

		btScalar physical_length = (from - result.m_hitPointWorld).length();	//the spring being compressed
		delta_t += timeStep;
		/*	//CHANGED MEANING OF x
		if(x == old_x)
		{
			return;
		}
			
		std::cout << "x:" << x << std::endl;
		std::cout << "t:" << delta_t << std::endl;*/
		
		btScalar x = rest.length() - physical_length;	//determines + or - for x
		std::cout << "x:" << x << std::endl;
		std::cout << "rest_length:" << rest.length() << std::endl;
		std::cout << "physic_length:" << physical_length << std::endl;
		btScalar spring_v = (x - old_x) / delta_t;

		btVector3 unit = to - from;	//length is full spring length
		unit.normalize();

		chasis->applyForce( -unit * (k* x - c*spring_v + 25 ) , from);
		chasis->activate();
		//chasis->applyForce(-unit * k*x, from);
		//chasis->applyForce(-unit * 100, from);
		std::cout << "kx" << k*x << std::endl;
		std::cout << "hit" << std::endl;
		old_x = x;
			
	}else
	{
		std::cout << "no hit" << std::endl;
		old_x = 0;
	}
	delta_t = 0;
}