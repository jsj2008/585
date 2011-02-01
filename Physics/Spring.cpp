#include "Spring.h"
#include <iostream>
#include "Common/SettingsFactory.h"

Spring::Spring(btRigidBody * const chasis, btVector3 const & from, btVector3 const & to, Physics * const physics) : 
chasis(chasis), from(from), to(to), physics(physics){
	debugger = physics->dynamicsWorld.getDebugDrawer();
	was_hit = false;
	current_weight = 0;
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

	//debugger->drawLine(from,from + rest ,btVector3(255,0,255));
	//debugger->drawLine(from,to ,btVector3(255,0,0));

	btCollisionWorld::ClosestRayResultCallback result(from, to);	
	physics->dynamicsWorld.rayTest( from, to, result);
	
	if(result.hasHit() )
	{		

		btVector3 physical_spring = (result.m_hitPointWorld - from);	//the spring as it is compressed
		btScalar physical_length = physical_spring.length();

		btScalar x = rest.length() - physical_length;

		//debugger->drawLine(from, from - spring_unit * x, btVector3(0,255,125));			
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
			// debugger->drawLine(from,from + projection*(force - 25)/4.0 ,btVector3(125,125,125));
			current_weight = force;
		}
		chasis->activate();
		old_x = x;		
		was_hit = true;
			
	}else
	{
		debugger->drawSphere(to - btVector3(0,0.25,0), 0.5, btVector3(255,0,0));
		debugger->drawLine(from, to, btVector3(0,255,0));
		was_hit = false;
		current_weight = 0;
	}
}