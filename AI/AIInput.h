#ifndef AIINPUT_H
#define AIINPUT_H

#include "UI/IInput.h"
#include "LevelAI.h"

class AIInput : public IInput {
public:

	LevelAI* levelAI;

	/*float XAxis;    
	float YAxis;
	bool AcceleratePressed;  
	bool BrakePressed;  
	bool EBrakePressed;*/

	AIInput();
	AIInput(LevelAI* levelAI);
	~AIInput();
	void step();
	void setLevelAI(LevelAI* levelAI);
};

#endif
