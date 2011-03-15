#ifndef JEEPACTOR_H
#define JEEPACTOR_H

#include "Common/Actor.h"
#include "JeepEngine.h"

class PhysObject;
class RenderObject;
class Physics;
class Spring;
class IInput;
class Sound;

class JeepActor : public Actor
{
public:
	JeepActor(PhysObject const &, RenderObject const &, Physics * const, IInput const * const = NULL, btVector3 const & pos = btVector3(0,0,0), btQuaternion const & vel = btQuaternion(0,0,0, 1));
	~JeepActor();
	void setOrientation(btQuaternion const &);
	void setPosition(btVector3 const &);
	void tick(btScalar);
	void render();
	void reset(btQuaternion const &, btVector3 const &);
	void registerAudio(Sound *);
	
	/*useful vectors*/
	btVector3 u;	//jeep is facing this way
	btVector3 up_axis;	//up of jeep
	btVector3 lateral;	//lateral of jeep
	btVector3 velocity;
	btScalar speed;
	btVector3 long_velocity;
	btScalar long_speed;
	float * audio_frame;
		
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
	IInput const * input;
	
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
	btScalar delta;
	
	
	//other states
	bool onGround;
	btScalar die_time;
	
};

#endif
