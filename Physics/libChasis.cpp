#include "libChasis.h"
#include "Common/SettingsFactory.h"
libChasis::libChasis() : PhysObject(10.0) //10.0 kg
{
	float const & width = LoadFloat("config/jeep_springs.xml", "width");
	float const & height = LoadFloat("config/jeep_springs.xml", "height");
	float const & depth = LoadFloat("config/jeep_springs.xml", "depth");
	fallInertia = new btVector3(0,0,0);
	shape = new btBoxShape(btVector3(width/2.0,height/2.0,depth/2.0));
}