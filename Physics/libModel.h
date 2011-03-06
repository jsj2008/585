#ifndef LIBMODEL_H
#define LIBMODEL_H

#include "PhysObject.h"

class Model;

class libModel : public PhysObject
{
public:
	libModel(Model const &, btScalar scale);
};

#endif