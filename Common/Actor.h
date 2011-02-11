/* 
 * File:   Actor.h
 * Author: oricohen
 *
 * Created on January 12, 2011, 8:58 PM
 */

#ifndef ACTOR_H
#define	ACTOR_H

#include "prelude.h"
#include "Physics/PhysObject.h"
#include <btBulletDynamicsCommon.h>

class Actor {
public:
	
    
    btVector3 pos;
	btQuaternion orientation;
    Real width;
    Real height;
	Real depth;
	Point initialVel;
	PhysObject const & physObject;	//physical information (mass, shape, etc...)
	
	Actor(PhysObject const &, Point pos = Point(0,0,0), Point vel = Point(0,0,0));
    virtual ~Actor(){};

	virtual void setOrientation(btQuaternion const &);
	virtual void setPosition(btVector3 const &);
};

#endif	/* ACTOR_H */

