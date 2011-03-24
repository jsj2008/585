#include "Sound.h"
#include <iostream>

ALuint Buffers[NUM_BUFFERS];
ALuint Sources[NUM_SOURCES];

ALfloat SourcesPos[NUM_SOURCES][9];
ALfloat SourcesVel[NUM_SOURCES][9];

ALfloat ListenerPos[] = {0.0f, 0.0f, 0.0f};
ALfloat ListenerVel[] = {0.0f, 0.0f, 0.0f};
//first three of orient are "at", last three are "up"
ALfloat ListenerOrient[] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f};



Sound::Sound()
{
	heartPitch = 1.0;
	enginePitch = 1.0;
	panicPitch = 1.0;
}

Sound::~Sound()
{

}

void Sound::KillALData(){
	alDeleteBuffers(NUM_BUFFERS, &Buffers[0]);
	alDeleteSources(NUM_SOURCES, &Sources[0]);
	alutExit();
}

void loadAudio(char * file, int index)
{
	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;
	
	#if defined(__APPLE__)
	alutLoadWAVFile(file,&format, &data, &size, &freq);
	#else
	alutLoadWAVFile(file, &format, &data, &size, &freq, &loop);
	#endif
	alBufferData(Buffers[index], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);
	
}

ALboolean Sound::LoadALData()
{
	//variables to load into

	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;

	//load wav data into a buffer
	alGenBuffers(NUM_BUFFERS, Buffers);

	if(alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	loadAudio("media/TribalGroove.wav", MUSIC);	
	loadAudio("media/ExoticBirds.wav", BIRDS);
	loadAudio("media/engine_idle.wav", ENGINE);
	loadAudio("media/cars031.wav",START);
	loadAudio("media/crash2.wav", CRASH);
	loadAudio("media/TribalDrums2.wav", PANIC);
	loadAudio("media/crashBig_die.wav", DIE);
	loadAudio("media/heart.wav", HEART);
	loadAudio("media/Roar.wav", ROAR);
	//bind buffers into audio sources
	alGenSources(NUM_SOURCES, Sources);

	if(alGetError() != AL_NO_ERROR)
	{
		return AL_FALSE;
	}

	alSourcei (Sources[MUSIC], AL_BUFFER, Buffers[MUSIC]);
	alSourcef (Sources[MUSIC], AL_PITCH, 1.0);
	alSourcef (Sources[MUSIC], AL_GAIN, 1.0);
	alSourcefv(Sources[MUSIC], AL_POSITION, SourcesPos[MUSIC]);
	alSourcefv(Sources[MUSIC], AL_VELOCITY, SourcesVel[MUSIC]);
	alSourcei(Sources[MUSIC], AL_LOOPING, AL_TRUE);

	alSourcei (Sources[BIRDS], AL_BUFFER, Buffers[BIRDS]);
	alSourcef (Sources[BIRDS], AL_PITCH, 1.0);
	alSourcef (Sources[BIRDS], AL_GAIN, 1.0);
	alSourcefv(Sources[BIRDS], AL_POSITION, SourcesPos[BIRDS]);
	alSourcefv(Sources[BIRDS], AL_VELOCITY, SourcesVel[BIRDS]);
	alSourcei(Sources[BIRDS], AL_LOOPING, AL_TRUE);

	alSourcei (Sources[ENGINE], AL_BUFFER, Buffers[ENGINE]);
	alSourcef (Sources[ENGINE], AL_PITCH, 1.0);
	alSourcef (Sources[ENGINE], AL_GAIN, 1.0);
	alSourcefv(Sources[ENGINE], AL_POSITION, SourcesPos[ENGINE]);
	alSourcefv(Sources[ENGINE], AL_VELOCITY, SourcesVel[ENGINE]);
	alSourcei(Sources[ENGINE], AL_LOOPING, AL_TRUE);

	alSourcei (Sources[START], AL_BUFFER, Buffers[START]);
	alSourcef (Sources[START], AL_PITCH, 1.0);
	alSourcef (Sources[START], AL_GAIN, 1.0);
	alSourcefv(Sources[START], AL_POSITION, SourcesPos[START]);
	alSourcefv(Sources[START], AL_VELOCITY, SourcesVel[START]);
	alSourcei(Sources[START], AL_LOOPING, AL_FALSE);

	alSourcei (Sources[CRASH], AL_BUFFER, Buffers[CRASH]);
	alSourcef (Sources[CRASH], AL_PITCH, 1.0);
	alSourcef (Sources[CRASH], AL_GAIN, 1.0);
	alSourcefv(Sources[CRASH], AL_POSITION, SourcesPos[CRASH]);
	alSourcefv(Sources[CRASH], AL_VELOCITY, SourcesVel[CRASH]);
	alSourcei(Sources[CRASH], AL_LOOPING, AL_FALSE);

	alSourcei (Sources[PANIC], AL_BUFFER, Buffers[PANIC]);
	alSourcef (Sources[PANIC], AL_PITCH, 1.0);
	alSourcef (Sources[PANIC], AL_GAIN, 1.0);
	alSourcefv(Sources[PANIC], AL_POSITION, SourcesPos[PANIC]);
	alSourcefv(Sources[PANIC], AL_VELOCITY, SourcesVel[PANIC]);
	alSourcei(Sources[PANIC], AL_LOOPING, AL_TRUE);

	alSourcei (Sources[DIE], AL_BUFFER, Buffers[DIE]);
	alSourcef (Sources[DIE], AL_PITCH, 1.0);
	alSourcef (Sources[DIE], AL_GAIN, 1.0);
	alSourcefv(Sources[DIE], AL_POSITION, SourcesPos[DIE]);
	alSourcefv(Sources[DIE], AL_VELOCITY, SourcesVel[DIE]);
	alSourcei(Sources[DIE], AL_LOOPING, AL_FALSE);

	alSourcei (Sources[HEART], AL_BUFFER, Buffers[HEART]);
	alSourcef (Sources[HEART], AL_PITCH, 1.0);
	alSourcef (Sources[HEART], AL_GAIN, 1.0);
	alSourcefv(Sources[HEART], AL_POSITION, SourcesPos[HEART]);
	alSourcefv(Sources[HEART], AL_VELOCITY, SourcesVel[HEART]);
	alSourcei(Sources[HEART], AL_LOOPING, AL_TRUE);

	alSourcei (Sources[ROAR], AL_BUFFER, Buffers[ROAR]);
	alSourcef (Sources[ROAR], AL_PITCH, 1.0);
	alSourcef (Sources[ROAR], AL_GAIN, 1.0);
	alSourcefv(Sources[ROAR], AL_POSITION, SourcesPos[ROAR]);
	alSourcefv(Sources[ROAR], AL_VELOCITY, SourcesVel[ROAR]);
	alSourcei(Sources[ROAR], AL_LOOPING, AL_FALSE);

	//do another check and return
	if(alGetError() != AL_NO_ERROR)
	{
		return AL_FALSE;
	}
	return AL_TRUE;
}

//pitch change is strictly hardcoded for the jeep engine at this point
void Sound::increasePitch(float pitchFactor)
{
	if(enginePitch + pitchFactor < 5.0)
	{
		alSourcef (Sources[ENGINE], AL_PITCH, enginePitch+=pitchFactor);
		alSourcePlay(Sources[ENGINE]);
	}
}

void Sound::decreasePitch(float pitchFactor)
{
	if(enginePitch - pitchFactor > 1.0)
	{
		alSourcef (Sources[ENGINE], AL_PITCH, enginePitch-=pitchFactor);
		alSourcePlay(Sources[ENGINE]);
	}
}

void Sound::beginLevel()
{
	//alSourcePlay(Sources[BIRDS]);
	alSourcePlay(Sources[MUSIC]);
}

void Sound::pauseTrack(ALuint track)
{
	alSourcePause(Sources[track]);
}

void Sound::stopTrack(ALuint track)
{
	alSourceStop(Sources[track]);
}

void Sound::playTrack(ALuint track)
{
	alSourcePlay(track); 
}

void Sound::playJump()
{
	//8.0 is magic -- just a high pitch
	enginePitch = 6.0;
	alSourcef (Sources[ENGINE], AL_PITCH, enginePitch);
	alSourcePlay(Sources[ENGINE]);
}

void Sound::playLand()
{
	alSourcePlay(Sources[CRASH]);
	//setting pitch back to a mid-speed sound
	enginePitch = 2.0;
	alSourcef (Sources[ENGINE], AL_PITCH, enginePitch);
	alSourcePlay(Sources[ENGINE]);
}

void Sound::playHeart()
{
	alSourcePlay(Sources[HEART]);
}

void Sound::playRoar()
{
	alSourcePlay(Sources[ROAR]);
}

void Sound::playHit()
{
	playTrack(Sources[CRASH]);
}

void Sound::playMusic()
{
	//want to add multiple random tracks in here
	playTrack(Sources[MUSIC]);
}

void Sound::playPanic()
{
	alSourceStop(Sources[BIRDS]);
	alSourceStop(Sources[MUSIC]);
	playTrack(Sources[PANIC]);
}

void Sound::increasePanic(float pitchFactor)
{
	if(panicPitch < 1.4){
		alSourcef (Sources[PANIC], AL_PITCH, panicPitch+=pitchFactor);
		alSourcePlay(Sources[PANIC]);
	}
}

void Sound::decreasePanic(float pitchFactor)
{
	if(panicPitch > 1.0){
		alSourcef (Sources[PANIC], AL_PITCH, panicPitch-=pitchFactor);
		alSourcePlay(Sources[PANIC]);
	}
}

void Sound::playAmbient()
{
	playTrack(Sources[BIRDS]);
}

void Sound::playEngine()
{
	playTrack(Sources[ENGINE]);
}

void Sound::playIgnition()
{
	playTrack(Sources[START]);
}

void Sound::playDie()
{
	alSourceStop(Sources[ENGINE]);
	alSourceStop(Sources[BIRDS]);
	alSourceStop(Sources[MUSIC]);
	playTrack(Sources[DIE]);
}

void Sound::playDiePanic()
{
	alSourceStop(Sources[PANIC]);
	alSourceStop(Sources[ENGINE]);
	alSourceStop(Sources[BIRDS]);
	alSourceStop(Sources[MUSIC]);

	playRoar();
}

void Sound::playRespawn()
{
	playTrack(Sources[START]);
	enginePitch = 1.0;
	alSourcef (Sources[ENGINE], AL_PITCH, enginePitch);
	playAmbient();
	playMusic();
}

void Sound::SetListenerValues(btScalar const * pos, btScalar const * vel, float const * orient)
{
	alListenerfv(AL_POSITION, pos);
	alListenerfv(AL_VELOCITY, vel);
	alListenerfv(AL_ORIENTATION, orient);
}

void Sound::SetSource(ALuint source, btScalar const * pos, btScalar const * vel)
{
	alSourcefv(source, AL_POSITION, SourcesPos[source]);
	alSourcefv(source, AL_VELOCITY, SourcesVel[source]);
	//alSourcePlay(Sources[ENGINE]);

}

/*
	//useful code for fading effects -- randomly plays crashes in a circle around the player
		if(play != AL_PLAYING)
		{
			double theta = (double) (rand() %360) * 3.14/ 180.0;

			SourcesPos[CRASH][0] = -float(cos(theta));
			SourcesPos[CRASH][1] = -float(rand()%2);
			SourcesPos[CRASH][2] = -float(sin(theta));

			alSourcefv(Sources[CRASH], AL_POSITION, SourcesPos[CRASH]);

			alSourcePlay(Sources[CRASH]);
		}*/	