#ifndef IENGINE_H
#define IENGINE_H

//#include "UI/IInput.h"
#include <btBulletDynamicsCommon.h>
#include "Common/prelude.h"

class IEngine
{

public:
	virtual ~IEngine(){}
	virtual void step(seconds const, btScalar const acclerate) =0;
	virtual btScalar getRPM() const = 0;
	//virtual void loadSettings(SettingsFile const &) =0;
	
};

#endif