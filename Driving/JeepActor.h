#ifndef JEEPACTOR_H
#define JEEPACTOR_H

#include "Common/Actor.h"
#include "Physics/PhysObject.h"
#include "Common/prelude.h"
#include <vector>
#include "Physics/Spring.h"

class JeepActor : public Actor
{
public:
	JeepActor(PhysObject const &, Physics * const, Point pos = Point(0,0,0), Point vel = Point(0,0,0));
	~JeepActor();
	void setOrientation(btQuaternion);
	void setPosition(btVector3);
	void tick(seconds);
	
private:
	Physics * const physics;
	btVector3 from[4];
	btVector3 origin_from[4];
	btVector3 to[4];
	btVector3 origin_to[4];
	typedef std::vector<Spring *> Springs;
	Springs springs;
	
};

#endif