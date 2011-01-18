#ifndef WINDOW_H
#define WINDOW_H
#include "Main/IsController.h"
#include "IsWindow.h"
#include <SDL.h>
#include <SDL_opengl.h>	//has to go first to avoid redef warning

class Window : public IsWindow
{
public:
	Window();
	virtual ~Window();
	void updateGL();
	void run(IsController * controller);
	int ScreenWidth(){return SCREEN_WIDTH;}
	int ScreenHeight(){return SCREEN_HEIGHT;}
	int ScreenDepth(){return SCREEN_DEPTH;}

private:
	
	static Uint32 Timer(Uint32 interval, void* param);
	
	static const int SCREEN_WIDTH = 1280;
	static const int SCREEN_HEIGHT = 1024;
	static const int SCREEN_DEPTH = 16;
	static const int TIMER = 1;

};
#endif