#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class HeightMap
{
public:
	HeightMap(std::string const &);	
	~HeightMap();
	unsigned char * map;
	int width;
	int height;
	int fakeWidth;
	int fakeHeight;
private:
	SDL_Surface * surface;
};

#endif