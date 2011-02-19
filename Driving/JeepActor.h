#ifndef JEEPACTOR_H
#define JEEPACTOR_H

#include "Common/Actor.h"
#include "Physics/PhysObject.h"
#include "Common/prelude.h"
#include <vector>
#include "Physics/Spring.h"
#include "UI/Input.h"
#include "Renderer/RenderObject.h"

class JeepActor : public Actor
{
public:
	JeepActor(PhysObject const &, RenderObject const &, Physics * const, Input const * const = NULL, btVector3 const & pos = btVector3(0,0,0), btVector3 const & vel = btVector3(0,0,0));
	~JeepActor();
	void setOrientation(btQuaternion const &);
	void setPosition(btVector3 const &);
	void tick(seconds);
	static void myTickCallback(btDynamicsWorld *world, btScalar timeStep);
		
private:
	Physics * const physics;
	btVector3 from[4];
	btVector3 origin_from[4];
	btVector3 to[4];
	btVector3 origin_to[4];
	typedef std::vector<Spring *> Springs;
	Springs springs;
	btRigidBody * chasis;
	Input const * input;
	
	bool isForward;
	bool isBackward;
	float const & offset_x;
	float const & offset_z;
	float const & spring_top;
	float const & spring_bottom;
	float const & mass;
	
};

#endif