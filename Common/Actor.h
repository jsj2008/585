/* 
 * File:   Actor.h
 * Author: oricohen
 *
 * Created on January 12, 2011, 8:58 PM
 */

#ifndef ACTOR_H
#define	ACTOR_H

#include "prelude.h"
#include "../Physics/PhysObject.h"

class Actor {
public:
    
    Point pos;
    Real width;
    Real height;
	Real depth;
	/*Point normal;
	Point heading;
	Point binormal;*/
	PhysObject * physObject;	//physical information (mass, shape, etc...)
	
	Actor();
	Actor(Real x, Real y, Real z);
    virtual ~Actor(){};
};

#endif	/* ACTOR_H */

