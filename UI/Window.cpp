#include "Window.h"
#include <iostream>
#include "Common/SettingsFactory.h"
#include "Common/Debug.h"
#include "Main/MainController.h"
#include "UI/Input.h"
#include "Renderer/Renderer.h"

Window::Window() : loading(true)
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
	Uint32 flags = SDL_OPENGL;// | SDL_FULLSCREEN;
	drawContext = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, flags);
	
	SDL_WM_SetCaption("SDL App", NULL);

	bInput = new Input();
	aInput=new Input();

	
}

void Window::stopLoading()
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	//sets up double buffering and 16-bit depth
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, SCREEN_DEPTH);
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);	//each channel is 8-bit
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	
	/*sets up OpenGL drawing context*/
	SDL_Surface * drawContext;
	Uint32 flags = SDL_OPENGL;// | SDL_FULLSCREEN;
	drawContext = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, flags);


	loading = false;
}

Uint32 Window::Timer(Uint32 interval, void* )
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
	glFlush();
	SDL_GL_SwapBuffers();
}

void Window::run(MainController * controller)
{     
	static int const & delay = LoadInt("config/window.xml", "delay");
	bool quit = false;
	int deltaT;
	Uint32 before = SDL_GetTicks();
	while(!quit) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear last drawing
		Uint32 after = SDL_GetTicks();
		controller->tick(after - before);
		deltaT = delay - (after-before);
		before = after;

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			LOG("Raw input: " << event.jaxis.value, "input");
			switch(event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			default:
				quit=aInput->UpdateInput(event);
			}
		}
		aInput->checkState();
		updateGL();
		if (deltaT > 0) SDL_Delay( deltaT );
	}
}