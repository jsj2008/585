#ifndef SPRING_H
#define SPRING_H

#include <btBulletDynamicsCommon.h>
#include <LinearMath/btIDebugDraw.h>

#include "Physics.h"
#include "Common/prelude.h"

class Spring
{
public:
	Spring(btRigidBody * const, btVector3 const & from, btVector3 const& to, Physics * const);
	void tick(seconds timestep, btVector3 const & pos);
	btScalar getWeight();
private:
	btRigidBody * const chasis;
	btVector3 const & from;
	btVector3 const & to;
	Physics * const physics;
	btIDebugDraw * debugger;
	btScalar old_x;
	bool was_hit;
	btScalar current_weight;
};

#endif