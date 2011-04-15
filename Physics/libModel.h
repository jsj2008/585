#ifndef LIBMODEL_H
#define LIBMODEL_H

#include "PhysObject.h"

class Model;

class libModel : public PhysObject
{
public:
	libModel(Model const &, btScalar scale, bool = false, float mass=0.0);
};

#endif