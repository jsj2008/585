#include "Models.h"
#include "RenderObject.h"

Models * Models::ptr = NULL;
Models::Models()
{
	if(ptr == NULL)
		ptr = this;
	
	RenderObject * jeepModel = new RenderObject("data/textures/jeep_uv2.png", "data/textures/blank.bmp", "models/Jeep/jeep7.obj");
	models.insert( ModelsPair("jeep", jeepModel));

	RenderObject * wheelModel = new RenderObject("data/textures/jeep_uv2.png", "data/textures/blank.bmp", "models/Jeep/wheel2.obj");
	models.insert( ModelsPair("wheel", wheelModel));
	
	
}

RenderObject const * Models::GetModel(std::string const & name)
{
	return ptr->models[name];
}
