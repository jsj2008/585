#include "Models.h"
#include "RenderObject.h"

Models * Models::ptr = NULL;
Models::Models()
{
	if(ptr == NULL)
		ptr = this;
	

	RenderObject * jeepModel1 = new RenderObject("data/textures/jeep_red.png", "data/textures/blank.bmp", "models/Jeep/jeep7.obj");
	models.insert( ModelsPair("jeep1", jeepModel1));

	RenderObject * jeepModel2 = new RenderObject("data/textures/jeep_dark_blue.png", "data/textures/blank.bmp", "models/Jeep/jeep7.obj");
	models.insert( ModelsPair("jeep2", jeepModel2));
	
	RenderObject * jeepModel3 = new RenderObject("data/textures/jeep_dark_purple.png", "data/textures/blank.bmp", "models/Jeep/jeep7.obj");
	models.insert( ModelsPair("jeep3", jeepModel3));

	RenderObject * jeepModel4 = new RenderObject("data/textures/jeep_dark_red.png", "data/textures/blank.bmp", "models/Jeep/jeep7.obj");
	models.insert( ModelsPair("jeep4", jeepModel4));

	RenderObject * jeepModel5 = new RenderObject("data/textures/jeep_dusty_blue.png", "data/textures/blank.bmp", "models/Jeep/jeep7.obj");
	models.insert( ModelsPair("jeep5", jeepModel5));

	RenderObject * jeepModel6 = new RenderObject("data/textures/jeep_green.png", "data/textures/blank.bmp", "models/Jeep/jeep7.obj");
	models.insert( ModelsPair("jeep6", jeepModel6));

	RenderObject * jeepModel7 = new RenderObject("data/textures/jeep_orange.png", "data/textures/blank.bmp", "models/Jeep/jeep7.obj");
	models.insert( ModelsPair("jeep7", jeepModel7));

	RenderObject * jeepModel8 = new RenderObject("data/textures/jeep_purple.png", "data/textures/blank.bmp", "models/Jeep/jeep7.obj");
	models.insert( ModelsPair("jeep8", jeepModel8));

	RenderObject * jeepModel9 = new RenderObject("data/textures/jeep_charcoal.png", "data/textures/blank.bmp", "models/Jeep/jeep7.obj");
	models.insert( ModelsPair("jeep9", jeepModel9));

	RenderObject * jeepModel10 = new RenderObject("data/textures/jeep_silver.png", "data/textures/blank.bmp", "models/Jeep/jeep7.obj");
	models.insert( ModelsPair("jeep10", jeepModel10));

	RenderObject * jeepModel11 = new RenderObject("data/textures/jeep_yellow.png", "data/textures/blank.bmp", "models/Jeep/jeep7.obj");
	models.insert( ModelsPair("jeep11", jeepModel11));

	RenderObject * wheelModel = new RenderObject("data/textures/jeep_uv2.png", "data/textures/blank.bmp", "models/Jeep/wheel2.obj");
	models.insert( ModelsPair("wheel", wheelModel));
	
	
}

RenderObject const * Models::GetModel(std::string const & name)
{
	return ptr->models[name];
}
