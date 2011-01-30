/*
INPUT.CPP
Input for either keyboard or controller, uses the even window.cpp gives it to do it.
Other classes can manually check each variable whether it is pressed down or not.
Button defaults are 1-3 or a,s,d for accelerating,braking and emergency braking.
Can be changed via the Change<input> functions

Todo: Scale Joystick properly according to axis value
	  Format Code correctly
	  Link with menu (to be created) in order to change controls accordingly.
	  


*/
#include "Input.h"
Input::Input(){
	SDL_Joystick *joystick;
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);
	XAxis=0;  
	YAxis=0;
	AcceleratePressed=false;  
	BrakePressed=false;  
	EBrakePressed=false;
	
	//user can change these key/button mappings via change button/key functions
	ACCELKEY='a';
	BRAKEKEY='s';
	EBRAKEKEY='d';
	MASTERQUITKEY='q';
	ACCELBUTTON=0;
	BRAKEBUTTON=1;
	EBRAKEBUTTON=2;
}

bool Input::UpdateInput(SDL_Event& event){ 

    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
        switch( event.key.keysym.sym )//key pressed
        {
            case SDLK_LEFT: XAxis=-1.0;  break;
            case SDLK_RIGHT: XAxis=1.0; break;
            case SDLK_DOWN: YAxis=-1.0;  break;
            case SDLK_UP: YAxis=1.0; break;
        }//end switch
		if(event.key.keysym.sym==ACCELKEY) AcceleratePressed=true;
		if(event.key.keysym.sym==BRAKEKEY) BrakePressed=true;
		if(event.key.keysym.sym==EBRAKEKEY) EBrakePressed=true;
		if(event.key.keysym.sym==MASTERQUITKEY) {SDL_JoystickClose(0);return true;}

	}//end if type
	else if( event.type == SDL_KEYUP )//key released
	{
        switch( event.key.keysym.sym )
		{
            case SDLK_LEFT: XAxis=0;  break;
            case SDLK_RIGHT: XAxis=0; break;
            case SDLK_DOWN: YAxis=0;  break;
            case SDLK_UP: YAxis=0; break;
        }
		if(event.key.keysym.sym==ACCELKEY) AcceleratePressed=false;
        if(event.key.keysym.sym==BRAKEKEY) BrakePressed=false;
		if(event.key.keysym.sym==EBRAKEKEY) EBrakePressed=false;

	}//end if type key released

	else if(event.type ==SDL_JOYAXISMOTION){
			if ( ( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ) ) 
			{
				if( event.jaxis.axis == 0) 
				{
					//TODO: Have a scale value to make controls move faster depending on axis
					if(event.jaxis.value<0)
					{
						XAxis=-1.0;//LEFT
					}
					if(event.jaxis.value>0)
					{
						XAxis=1.0;//RIGHT
					}
				}//end left/right joystick control

				if( event.jaxis.axis == 1) //sitting here if needed
				{
					if(event.jaxis.value<0)
					{
						YAxis=-1.0;  //DOWN
					}
					if(event.jaxis.value>0)
					{
						YAxis=1.0;  //UP
					}
				}//end updown movement joystick control
			}//end joystick axis control
	}//end else if type
	else if(event.type ==SDL_JOYBUTTONDOWN){
		//TODO:Figure out which xbox button is which 
		if(event.jbutton.button==ACCELBUTTON) AcceleratePressed=true;
        if(event.jbutton.button==BRAKEBUTTON) BrakePressed=true;
		if(event.jbutton.button==EBRAKEBUTTON) EBrakePressed=true;
	}//end else if type button
	else if(event.type ==SDL_JOYBUTTONUP){
		//TODO:Figure out which xbox button is which 
		if(event.jbutton.button==ACCELBUTTON) AcceleratePressed=false;
        if(event.jbutton.button==BRAKEBUTTON) BrakePressed=false;
		if(event.jbutton.button==EBRAKEBUTTON) EBrakePressed=false;
	}//end else if type button


	return false;//tell quit to quit
}//end startInput() function


//Change keys/buttons functions
void Input::changeAccelKey(char aKey)
{
	ACCELKEY=aKey;
}
void Input::changeBrakeKey(char aKey)
{
	BRAKEKEY=aKey;
}
void Input::changeEBrakeKey(char aKey)
{
	EBRAKEKEY=aKey;
}
void Input::changeMasterQuitKey(char aKey)
{
	MASTERQUITKEY=aKey;
}
void Input::changeAccelButton(Uint8 aButton)
{
	ACCELBUTTON=aButton;
}
void Input::changeBrakeButton(Uint8 aButton)
{
	BRAKEBUTTON=aButton;
}
void Input::changeEBrakeButton(Uint8 aButton)
{
	EBRAKEBUTTON=aButton;
}

