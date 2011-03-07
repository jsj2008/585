#include "PhysicsFactory.h"

Physics * PhysicsFactory::newPhysics(ActorList const & actors, btIDebugDraw & debug)
{
	return new Physics(actors, debug);
}