#ifndef SOUND_H
#define SOUND_H
#include <math.h>
#include <sound_dependencies/alc.h>
#include <sound_dependencies/al.h>
#include <sound_dependencies/alut.h>
#include <btBulletDynamicsCommon.h>


#define NUM_BUFFERS 9
#define NUM_SOURCES 9

#define MUSIC	0
#define BIRDS	1
#define ENGINE	2
#define START	3
#define CRASH	4
#define PANIC	5
#define DIE		6
#define HEART	7
#define ROAR	8

class Sound
{
	public:

	float heartPitch;
	float enginePitch;
	float panicPitch;

	Sound();
	~Sound();

	void KillALData();
	ALboolean LoadALData();
	void increasePitch(float pitchFactor);
	void decreasePitch(float pitchFactor);
	
	void pauseTrack(ALuint track);
	void stopTrack(ALuint track);
	void playTrack(ALuint track);

	void SetListenerValues(btScalar const *, btScalar const *, float const *);

	void beginLevel();
	void playHeart();
	void playRoar();
	void playJump();
	void playLand();
	void playHit();
	void playMusic();
	void playPanic();
	void increasePanic(float pitchFactor);
	void decreasePanic(float pitchFactor);
	void playAmbient();
	void playEngine();
	void playIgnition();
	void playDie();
	void playDiePanic();
	void playRespawn();

};
#endif