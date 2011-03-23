#ifndef WINDOW_H
#define WINDOW_H

#include "IWindow.h"
#include <SDL.h>
#define NO_SDL_GLEXT
#include <GL/glew.h>
#include "SDL_opengl.h"

//#define LOW_RES

class Input;
class MainController;

class Window : public IWindow
{
public:
	Window();
	virtual ~Window();
	void updateGL();
	void run(MainController * controller);
	void stopLoading();
	int ScreenWidth() const {return SDL_GetVideoSurface()->w;}
	int ScreenHeight() const {return SDL_GetVideoSurface()->h;}
	int ScreenDepth() const {return SCREEN_DEPTH;}
	Input *aInput;
	Input *bInput;

private:
	static Uint32 Timer(Uint32 interval, void* param);
	
	#ifdef LOW_RES
	static const int SCREEN_WIDTH = 800;
	static const int SCREEN_HEIGHT = 600;
	#else
	static const int SCREEN_WIDTH = 1024;
	static const int SCREEN_HEIGHT = 768;
	#endif
	static const int SCREEN_DEPTH = 16;
	static const int TIMER = 1;

};
#endif