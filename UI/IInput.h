#ifndef IINPUT_H
#define IINPUT_H

class IInput {
public:
	float XAxis;    
	float YAxis;
	bool AcceleratePressed;  
	bool BrakePressed;  
	bool EBrakePressed;
	virtual void restart()=0;
};

#endif
