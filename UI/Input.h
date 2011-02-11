#ifndef INPUT_H
#define INPUT_H


#include "Main/IController.h"
#include <SDL.h>
class Input
{
public:

	float XAxis;    
	float YAxis;
	bool AcceleratePressed;  
	bool BrakePressed;  
	bool EBrakePressed;
	char ACCELKEY;
	char BRAKEKEY;
	char EBRAKEKEY;
	char MASTERQUITKEY;
	Uint8 ACCELBUTTON;
	Uint8 BRAKEBUTTON;
	Uint8 EBRAKEBUTTON;

	Input();
	~Input();
	bool UpdateInput(SDL_Event& event);
	void changeAccelKey(char aKey);
	void changeBrakeKey(char aKey);
	void changeEBrakeKey(char aKey);
	void changeMasterQuitKey(char aKey);
	void changeAccelButton(Uint8 aButton);
	void changeBrakeButton(Uint8 aButton);
	void changeEBrakeButton(Uint8 aButton);
	
	
};



#endif // Input_H