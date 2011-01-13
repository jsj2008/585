/* 
 * File:   Actor.h
 * Author: oricohen
 *
 * Created on January 12, 2011, 8:58 PM
 */

#ifndef ACTOR_H
#define	ACTOR_H

#include "prelude.h"

class Actor {
public:
    
    Point pos;
    Real width;
    Real height;

    virtual ~Actor(){};
};

#endif	/* ACTOR_H */

