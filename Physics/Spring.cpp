#include "Spring.h"

Spring::Spring(btRigidBody * const chasis, btVector3 const & from, btVector3 const & to, Physics * const physics) : 
chasis(chasis), from(from), to(to), physics(physics){
	debugger = physics->dynamicsWorld.getDebugDrawer();
}

void Spring::tick(seconds timeStep)
{
	static btScalar old_x = 0;
	static btScalar k=150.0;
	static btScalar mass = 10.0;
	static btScalar c = 1.5*sqrt(k/mass);		
	
	debugger->drawLine(from,to ,btVector3(255,0,0));
	
	btCollisionWorld::ClosestRayResultCallback result(from, to);	
	physics->dynamicsWorld.rayTest( from, to, result);
	if(result.hasHit() )
	{		
		btScalar x = (to - result.m_hitPointWorld).length();
		btVector3 unit = to - from;
		unit.normalize();
		chasis->applyForce( -unit * (k* x - c*(abs(old_x - x))/timeStep) , from);
		old_x = x;
			
	}else
	{
		old_x = 0;
	}
}