#include "HeightMap.h"
#include <iostream>

typedef unsigned char uchar;

HeightMap::HeightMap(std::string const & filename)
{
	surface = IMG_Load(filename.c_str() );
	if(!surface)
	{
		std::cout << "Could not load heightmap " << filename << std::endl;
		exit(1);
		return;
	}
	
	int stride = surface->format->BytesPerPixel;

	width = surface->w;
	height = surface->h;
	map = new uchar [width * height];
	unsigned char * tmap = static_cast<unsigned char *>(surface->pixels);
	for(int i=0; i<width * height; i++)
	{
		map[i] = tmap[i*stride];
	}
	
	SDL_FreeSurface(surface);
}

HeightMap::~HeightMap()
{
}