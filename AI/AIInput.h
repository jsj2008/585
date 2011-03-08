#ifndef AIINPUT_H
#define AIINPUT_H

#include "UI/IInput.h"

class btVector3;
class JeepActor;

class AIInput : public IInput {
public:

	/*float XAxis;    
	float YAxis;
	bool AcceleratePressed;  
	bool BrakePressed;  
	bool EBrakePressed;*/

	AIInput();
	~AIInput();
	void step(JeepActor* jeep, btVector3 const & pathDir1, btVector3 const & pathDir2, btVector3 const & trackVector, btVector3 const & segmentVec1, btVector3 const & segmentVec2);
};

#endif
