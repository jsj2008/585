/* 
 * File:   prelude.h
 * Author: oricohen
 *
 * Created on January 12, 2011, 8:54 PM
 */

#ifndef PRELUDE_H
#define	PRELUDE_H

#include <list>
class Actor;

typedef float Real;
typedef std::list<Actor *> ActorList;
typedef float seconds;

typedef struct
{
    Real x;
    Real y;
    Real z;
} Point;


#endif	/* PRELUDE_H */

