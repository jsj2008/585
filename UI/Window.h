#ifndef WINDOW_H
#define WINDOW_H
#include "Main/IController.h"
#include "IWindow.h"
#include "Input.h"
#include <SDL.h>
#include <SDL_opengl.h>	//has to go first to avoid redef warning

class Window : public IWindow
{
public:
	Window();
	virtual ~Window();
	void updateGL();
	void run(IController * controller);
	int ScreenWidth() const {return SDL_GetVideoSurface()->w;}
	int ScreenHeight() const {return SDL_GetVideoSurface()->h;}
	int ScreenDepth() const {return SCREEN_DEPTH;}
	Input *aInput;

private:
	
	static Uint32 Timer(Uint32 interval, void* param);
	
	static const int SCREEN_WIDTH = 800;
	static const int SCREEN_HEIGHT = 600;
	static const int SCREEN_DEPTH = 16;
	static const int TIMER = 1;

};
#endif