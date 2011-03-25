#ifndef AIINPUT_H
#define AIINPUT_H

#include "UI/IInput.h"
#include "Driving/JeepManager.h"

class btVector3;
class JeepActor;

class AIInput : public IInput {
public:
	typedef std::vector<JeepActor *> Jeeps;

	AIInput();
	~AIInput();
	void step(JeepActor* jeep, Jeeps allJeeps, btVector3 const & pathDir1, btVector3 const & pathDir2, btVector3 const & trackVector, btVector3 const & segmentVec1, btVector3 const & segmentVec2);
	void restart();
private:
	int recovering;
};

#endif
