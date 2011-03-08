#include "libModel.h"
#include "Renderer/Model.h"
#include "Common/Debug.h"
libModel::libModel(Model const & model, btScalar scale) : PhysObject(0.0) //5.0 kg
{
	fallInertia = new btVector3(0,0,0);
	
	btConvexHullShape * t_shape = new btConvexHullShape();
	
	for(std::vector<btVector3 *>::const_iterator itr = model.vertices.begin(); itr != model.vertices.end(); ++itr )
	{
		t_shape->addPoint( (**itr) * scale );
	}
	
	shape = t_shape;
	
	// shape = new btCapsuleShape(10.0, 15.0);
	// shape = new btCylinderShape( btVector3(70.0, 80.0, 70.0) );
}