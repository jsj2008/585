#include "Window.h"
#include <iostream>

Window::Window()
{
	SDL_Init(SDL_INIT_EVERYTHING);	//initialize timer, audio, video, cd_rom, and joystick

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	//sets up double buffering and 16-bit depth
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, SCREEN_DEPTH);
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);	//each channel is 8-bit
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	
	/*sets up OpenGL drawing context*/
	SDL_Surface * drawContext;
	Uint32 flags = SDL_OPENGL;//| SDL_FULLSCREEN;
	drawContext = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, flags);
	
	/*setup timer*/
	SDL_TimerID timer;
	timer = SDL_AddTimer(10, Timer, this);
}

Uint32 Window::Timer(Uint32 interval, void* param)
{
	SDL_Event event;
	
	event.type = SDL_USEREVENT;
	event.user.code = TIMER;
	event.user.data1 = 0;
	event.user.data2 = 0;
	
	SDL_PushEvent(&event);
	
	return interval;
}

Window::~Window(){}

void Window::updateGL()
{
	SDL_GL_SwapBuffers();
}

void Window::run(IsController * controller)
{     
     
	bool quit = false;
	while(!quit) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_USEREVENT:
				if(event.user.code == TIMER)
				{
					controller->tick();
				}
				break;
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == 'q') quit = true;
				break;
			}
		}
		
		controller->yield();	//gives control to main loop
		SDL_Delay(30);
	}
}