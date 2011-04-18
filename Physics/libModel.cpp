#include "libModel.h"
#include "Renderer/Model.h"
#include "Common/Debug.h"
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btTriangleMesh.h>

libModel::libModel(Model const & model, btScalar scale, bool concave, float mass, bool isCourse) : PhysObject(mass, isCourse)
{
    if(mass > 0 && concave)
    {
        LOG("Model wants to be concave and falling!!! can't do it!!!", "libModel");
        exit(137);
    }
    
    fallInertia = new btVector3(0,0,0);
    
    if(!concave)
    {
	
    	btConvexHullShape * t_shape = new btConvexHullShape();
	
    	for(std::vector<btVector3 *>::const_iterator itr = model.vertices.begin(); itr != model.vertices.end(); ++itr )
    	{
    		t_shape->addPoint( (**itr) * scale );
    	}
	
    	shape = t_shape;
	}else
	{
	    if(model.faces[0].vertices.size() == 4)
	    {
            LOG("Model uses QUADS but wants to be cocave!!!", "libModel");
            exit(139);
	    }
	    
        btTriangleMesh * mesh = new btTriangleMesh();
        
    	for(std::vector<Face>::const_iterator itr = model.faces.begin(); itr != model.faces.end(); ++itr )
    	{
    	 
            mesh->addTriangle( *(model.vertices[ itr->vertices[0] -1 ]) * scale, *(model.vertices[ itr->vertices[1] -1 ])* scale, *(model.vertices[ itr->vertices[2] -1 ])* scale );
            
    	}
    	
    	
        btBvhTriangleMeshShape * t_shape = new btBvhTriangleMeshShape(mesh, true);
        
        shape = t_shape;
    	
	}
	
}