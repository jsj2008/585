#include "PhysicsFactory.h"

IPhysics * PhysicsFactory::newPhysics(ActorList const & actors, btIDebugDraw & debug)
{
	return new Physics(actors, debug);
}