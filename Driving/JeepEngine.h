#ifndef JEEPENGINE_H
#define JEEPENGINE_H
#include "IEngine.h"

class JeepEngine
{
public:
	JeepEngine();
	void step(seconds const, btScalar const acclerate );
	btScalar getRPM() const;

private:
	btScalar m_torque;
	static const int MIN_TORQUE = 0;
	static const int MAX_TORQUE = 60000;
	
};

#endif