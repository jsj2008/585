#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <SDL.h>
#include <string>

class HeightMap
{
public:
	HeightMap(std::string const &);	
	~HeightMap();
	unsigned char * map;
	int width;
	int height;
private:
	SDL_Surface * surface;
};

#endif