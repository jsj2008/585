#ifndef SPRING_H
#define SPRING_H

#include <btBulletDynamicsCommon.h>
#include <LinearMath/btIDebugDraw.h>

#include "Physics.h"
#include "Common/prelude.h"
#include "Renderer/RenderObject.h"
#include "Common/Actor.h"

class Spring
{
public:
	Spring(btRigidBody * const, btVector3 const & from, btVector3 const& to, Physics * const);
	void tick(seconds timestep, btVector3 const & pos, btScalar steer_angle);
	btScalar getWeight();
	btVector3 getForce(btScalar torque, btVector3 const & linear_velocity, btVector3 const & tire_direction);
	btVector3 getLateralForce(btVector3 const & linear_velocity, btVector3 const & tire_direction);
	btVector3 const & getFriction(btVector3 const & linear_velocity, btVector3 const & angular_velocity) const;
	btVector3 plane_normal;
	void render();
	
private:
	
	btScalar slip_ratio_lookup(btScalar);
	
	btRigidBody * const chasis;
	btVector3 const & from;
	btVector3 const & to;
	Physics * const physics;
	btIDebugDraw * debugger;
	btScalar old_x;
	bool was_hit;
	btScalar current_weight;
	btScalar wheel_speed;
	btScalar const & wheel_radius;
	RenderObject wheelModel;
	Actor * wheel_actor;
	btQuaternion current_direction;
	btVector3 const & planeProjection(btVector3 const & tire_direction) const;
	btVector3 hitPoint;
};

#endif