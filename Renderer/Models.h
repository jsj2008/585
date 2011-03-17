#ifndef MODELS_H
#define MODELS_H

#include <map>

class RenderObject;

class Models
{
public:
	Models();
	static RenderObject const * GetModel(std::string const &);
	
private:
	typedef std::pair<std::string const, RenderObject const *> ModelsPair;
	
	std::map<std::string, RenderObject const *> models;
	static Models * ptr;
};

#endif