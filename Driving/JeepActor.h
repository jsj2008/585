#ifndef JEEPACTOR_H
#define JEEPACTOR_H

#include "Common/Actor.h"
#include "Physics/PhysObject.h"
#include "Common/prelude.h"
#include <vector>
#include "Physics/Spring.h"
#include "UI/Input.h"
#include "Renderer/RenderObject.h"
#include <LinearMath/btIDebugDraw.h>
#include "JeepEngine.h"

class JeepActor : public Actor
{
public:
	JeepActor(PhysObject const &, RenderObject const &, Physics * const, Input const * const = NULL, btVector3 const & pos = btVector3(0,0,0), btVector3 const & vel = btVector3(0,0,0));
	~JeepActor();
	void setOrientation(btQuaternion const &);
	void setPosition(btVector3 const &);
	void tick(seconds);
	static void myTickCallback(btDynamicsWorld *world, btScalar timeStep);
	void render();
		
private:
	
	inline btVector3 long_friction();
	inline btVector3 lateral_friction(btScalar);
	inline btVector3 air_resistance(); 
	inline btVector3 update_tires();
	inline void isOnGround();
	
	Physics * const physics;
	btVector3 from[4];
	btVector3 origin_from[4];
	btVector3 to[4];
	btVector3 origin_to[4];
	typedef std::vector<Spring *> Springs;
	Springs springs;
	btRigidBody * chasis;
	Input const * input;
	
	JeepEngine engine;	
	
	float const & offset_x;
	float const & offset_z;
	float const & spring_top;
	float const & spring_bottom;
	static float mass;
	float const & c_drag;
	float const & c_roll;
	float const & c_roll2;
	static float gravity;
	float const & max_rotate;
	float const & turn_time;
	
	/*useful vectors*/
	btVector3 u;	//jeep is facing this way
	btVector3 up_axis;	//up of jeep
	btVector3 lateral;	//lateral of jeep
	btVector3 velocity;
	btScalar speed;
	btVector3 long_velocity;
	btScalar long_speed;
	
	//other states
	bool onGround;
	
};

#endif
