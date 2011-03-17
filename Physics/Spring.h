#ifndef SPRING_H
#define SPRING_H

#include "Renderer/RenderObject.h"

class Actor;
class btIDebugDraw;
class Physics;

class Spring
{
public:
	Spring(btRigidBody * const, btVector3 const & from, btVector3 const& to, Physics * const);
	void tick(seconds timestep, btVector3 const & pos, btScalar steer_angle);
	btScalar getWeight();
	btVector3 getForce(btScalar torque, btVector3 const & linear_velocity, btVector3 const & tire_direction);
	btVector3 getLateralForce(btVector3 const & linear_velocity, btVector3 const & tire_direction);
	btVector3 getFriction(btVector3 const & linear_velocity, btVector3 const & angular_velocity) const;
	btVector3 plane_normal;
	void render();
	void spinTire(btVector3 const &, btScalar);
	
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
	RenderObject const * wheelModel;
	Actor * wheel_actor;
	btQuaternion current_direction;
	btVector3 planeProjection(btVector3 const & tire_direction) const;
	btVector3 hitPoint;
	btScalar physical_length;

	btScalar tire_rot;
};

#endif