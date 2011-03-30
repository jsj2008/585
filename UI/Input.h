#ifndef INPUT_H
#define INPUT_H

#include "IInput.h"
#include "Main/IController.h"
#include <SDL.h>
class Input : public IInput {
public:

	void restart(){};

/*	float XAxis;    
	float YAxis;
	bool AcceleratePressed;  
	bool BrakePressed;  
	bool EBrakePressed;*/
	char ACCELKEY;
	char BRAKEKEY;
	char EBRAKEKEY;
	char MASTERQUITKEY;
	Uint8 STARTBUTTON;
	Uint8 BBUTTON;
	Uint8 YESBUTTON;
	Uint8 BRAKEBUTTON;
	Uint8 EBRAKEBUTTON;
	Uint8 LBUTTON;
	SDL_Joystick *joystick;
	
    bool Escape;
    bool Enter;
	bool LButton;
	bool BButton;

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
	void checkState();
	
};



#endif // Input_H
