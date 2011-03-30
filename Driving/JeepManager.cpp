#include "Common/Debug.h"
#include "JeepManager.h"
#include "JeepActor.h"
#include "AI/LevelAI.h"
#include "Main/MainController.h"
#include "Physics/Physics.h"
#include "AI/AIInput.h"
#include "UI/Input.h"
#include "Renderer/Models.h"

JeepManager::JeepManager() :
jeepModel1( Models::GetModel("jeep1") ),
jeepModel2( Models::GetModel("jeep2") ),
jeepModel3( Models::GetModel("jeep3") ),
jeepModel4( Models::GetModel("jeep4") ),
jeepModel5( Models::GetModel("jeep5") ),
jeepModel6( Models::GetModel("jeep6") ),
jeepModel7( Models::GetModel("jeep7") ),
jeepModel8( Models::GetModel("jeep8") ),
jeepModel9( Models::GetModel("jeep9") ),
jeepModel10( Models::GetModel("jeep10") ),
jeepModel11( Models::GetModel("jeep11") )
{}

void JeepManager::initialize(Physics * physics, Input * playerInput)
{
	float jeepX			= LoadFloat("config/start.xml", "jeepX");
	float jeepY			= LoadFloat("config/start.xml", "jeepY");
	float jeepZ 		= LoadFloat("config/start.xml", "jeepZ");
	float jeepRotation	= LoadFloat("config/start.xml", "jeepRotation");

	btQuaternion rotation(btVector3(0,1,0), jeepRotation);	//initial rotation
	
	human = new JeepActor(mChasis, jeepModel1, physics, playerInput, btVector3(jeepX, jeepY, jeepZ), rotation );
	human->isHuman = true;
	human->ID = 10;
	//MainController::addActor(human);
	num_players = LoadInt("config/ai.xml","num_players");
	
	for (int i = 0; i < num_players; ++i) {
		aiInputs.push_back(new AIInput());
	}
	// We WILL fix this garbage
		JeepActor* jeep1 = new JeepActor(mChasis, jeepModel2, physics, aiInputs[0], btVector3(jeepX + (10*0 + 10), jeepY, jeepZ), rotation );
		jeep1->setOrientation( rotation );
		jeep1->ID = 0;
		aiJeeps.push_back(jeep1);
		JeepActor* jeep2 = new JeepActor(mChasis, jeepModel3, physics, aiInputs[1], btVector3(jeepX + (10*1 + 10), jeepY, jeepZ), rotation );
		jeep2->setOrientation( rotation );
		jeep2->ID = 1;
		aiJeeps.push_back(jeep2);
		JeepActor* jeep3 = new JeepActor(mChasis, jeepModel4, physics, aiInputs[2], btVector3(jeepX + (10*2 + 10), jeepY, jeepZ), rotation );
		jeep3->setOrientation( rotation );
		jeep3->ID = 2;
		aiJeeps.push_back(jeep3);
		JeepActor* jeep4 = new JeepActor(mChasis, jeepModel5, physics, aiInputs[3], btVector3(jeepX + (10*3 + 10), jeepY, jeepZ), rotation );
		jeep4->setOrientation( rotation );
		jeep4->ID = 3;
		aiJeeps.push_back(jeep4);
		JeepActor* jeep5 = new JeepActor(mChasis, jeepModel6, physics, aiInputs[4], btVector3(jeepX + (10*4 + 10), jeepY, jeepZ), rotation );
		jeep5->setOrientation( rotation );
		jeep5->ID = 4;
		aiJeeps.push_back(jeep5);
		JeepActor* jeep6 = new JeepActor(mChasis, jeepModel7, physics, aiInputs[5], btVector3(jeepX + (10*5 + 10), jeepY, jeepZ), rotation );
		jeep6->setOrientation( rotation );
		jeep6->ID = 5;
		aiJeeps.push_back(jeep6);
		JeepActor* jeep7 = new JeepActor(mChasis, jeepModel8, physics, aiInputs[6], btVector3(jeepX + (10*6 + 10), jeepY, jeepZ), rotation );
		jeep7->setOrientation( rotation );
		jeep7->ID = 6;
		aiJeeps.push_back(jeep7);
		JeepActor* jeep8 = new JeepActor(mChasis, jeepModel9, physics, aiInputs[7], btVector3(jeepX + (10*7 + 10), jeepY, jeepZ), rotation );
		jeep8->setOrientation( rotation );
		jeep8->ID = 7;
		aiJeeps.push_back(jeep8);
		JeepActor* jeep9 = new JeepActor(mChasis, jeepModel10, physics, aiInputs[8], btVector3(jeepX + (10*8 + 10), jeepY, jeepZ), rotation );
		jeep9->setOrientation( rotation );
		jeep9->ID = 8;
		aiJeeps.push_back(jeep9);
		JeepActor* jeep10 = new JeepActor(mChasis, jeepModel11, physics, aiInputs[9], btVector3(jeepX + (10*9 + 10), jeepY, jeepZ), rotation );
		jeep10->setOrientation( rotation );
		jeep10->ID = 9;
		aiJeeps.push_back(jeep10);


	levelAI = new LevelAI(aiJeeps, human);

	physics->dynamicsWorld.setInternalTickCallback(tickCallback, static_cast<void *>(this), true );
    freezeAt(btVector3(jeepX, jeepY, jeepZ));
    
    physics = physics;
	
}

void JeepManager::restart()
{
	/*TODO: reset AI segments*/
	float jeepX			= LoadFloat("config/start.xml", "jeepX");
	float jeepY			= LoadFloat("config/start.xml", "jeepY");
	float jeepZ 		= LoadFloat("config/start.xml", "jeepZ");
	float jeepRotation	= LoadFloat("config/start.xml", "jeepRotation");

	btQuaternion rotation(btVector3(0,1,0), jeepRotation);	//initial rotation
	
	human->reset(rotation, btVector3(jeepX, jeepY, jeepZ));
	for (int i = 0; i < num_players; ++i) {
		aiJeeps[i]->reset(rotation, btVector3(jeepX + (10*i + 10), jeepY, jeepZ) );
		this->aiInputs[i]->restart();
	}
	levelAI->restart();
	
    freezeAt(btVector3(jeepX, jeepY, jeepZ));
	
}

//respawn a single jeep
void JeepManager::restartJeep(JeepActor * jeep)
{
	btVector3 pos = levelAI->getPlayerPosition(jeep->ID);
	
	float jeepRotation = levelAI->getPlayerRotation(jeep->ID);
	
	btQuaternion rotation(btVector3(0,1,0), jeepRotation);
	jeep->reset( rotation, pos);	
	jeep->release();
}

void JeepManager::release()
{
    human->release();
    for (int i = 0; i < num_players; ++i) {
        aiJeeps[i]->release();
	}
    
}

void JeepManager::freezeAt(btVector3 const & pos)
{
    human->freezeAt(pos);
    int num_players = LoadInt("config/ai.xml","num_players");
    for (int i = 0; i < num_players; ++i) {
        aiJeeps[i]->freezeAt(pos);
	}
}

void JeepManager::hitDetection(btDynamicsWorld * world)
{
    	int numManifolds = world->getDispatcher()->getNumManifolds();
    	for (int i=0;i<numManifolds;i++)
    	{
    		btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
    		btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
    		btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

    		int numContacts = contactManifold->getNumContacts();
    		for (int j=0;j<numContacts;j++)
    		{
    			btManifoldPoint& pt = contactManifold->getContactPoint(j);
                btScalar distance = pt.getDistance();
    			if (distance<0.f)
    			{
                    void * a = obA->getUserPointer();
                    void * b = obB->getUserPointer();
                                            
                    const btVector3& ptA = pt.getPositionWorldOnA();
    				const btVector3& ptB = pt.getPositionWorldOnB();
    				const btVector3& normalOnB = pt.m_normalWorldOnB;
    				
                    btScalar impulse = pt.getAppliedImpulse();
                        				
    				if(a != NULL)
                        static_cast<JeepActor*>(a)->hitObject( impulse, ptA );
                    
                    if(b != NULL)
                        static_cast<JeepActor*>(b)->hitObject( impulse, ptB );
                    LOG("impulse " << pt.getAppliedImpulse(), "hit");
    				
    			}
    		}
    	}
}

void JeepManager::tickCallback(btDynamicsWorld *world, btScalar timeStep)
{
	JeepManager * m = static_cast<JeepManager *>(world->getWorldUserInfo());
    m->hitDetection(world);
    
	m->human->tick(timeStep);
	for(Jeeps::iterator itr = m->aiJeeps.begin(); itr != m->aiJeeps.end(); ++itr)
		(*itr)->tick(timeStep);
}

void JeepManager::renderTick() {
	levelAI->step();
	human->render();
	int c = 0;
	for (Jeeps::iterator itr = aiJeeps.begin(); itr != aiJeeps.end(); ++itr, c++) {
		(*itr)->render();
		btVector3 pathDir1 = levelAI->getPathDirection(0, c);
		btVector3 pathDir2 = levelAI->getPathDirection(1, c);
		btVector3 trackVector = levelAI->getVectorToTrack(c);
		btVector3 segmentVec1 = levelAI->getVectorToSeg(1, c);
		btVector3 segmentVec2 = levelAI->getVectorToSeg(2, c);
		Jeeps allJeeps = aiJeeps;
		allJeeps.push_back(human);
		aiInputs[c]->step((*itr), allJeeps, pathDir1, pathDir2, trackVector, segmentVec1, segmentVec2, levelAI->getPlayerPlace(c));
	}
	
    
}

void JeepManager::startEngines()
{
	human->startEngine();
	for (Jeeps::iterator itr = aiJeeps.begin(); itr != aiJeeps.end(); ++itr) {
		(*itr)->startEngine();
	}
}


JeepActor* JeepManager::getHuman() {
	return human;
}

JeepManager::Jeeps JeepManager::getAIs() {
	return aiJeeps;
}

btVector3 JeepManager::getPlayerPos(int p) const {
	if (p < aiJeeps.size()) return aiJeeps[p]->pos;
	else return human->pos;
}

int JeepManager::getPlayerPlace(int p) {
	return levelAI->getPlayerPlace(p);
}
