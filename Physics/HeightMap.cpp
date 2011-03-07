#include "HeightMap.h"
#include <iostream>

HeightMap::HeightMap(std::string const & filename)
{
	surface = SDL_LoadBMP(filename.c_str() );
	if(!surface)
	{
		std::cout << "Could not load heightmap " << filename << std::endl;
		exit(1);
		return;
	}
	
	width = surface->w;
	height = surface->h;
	map = static_cast<unsigned char *>(surface->pixels);	
	
}

HeightMap::~HeightMap()
{
	if (surface) 
		SDL_FreeSurface(surface);
}