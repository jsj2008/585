#include "Sound.h"
#include <cstdlib>
#include "Common/Debug.h"

//handy for debugging
std::string GetALErrorString(ALenum err)
{
    switch(err)
    {
        case AL_NO_ERROR:
            return "AL_NO_ERROR";
        break;

        case AL_INVALID_NAME:
            return "AL_INVALID_NAME";
        break;

        case AL_INVALID_ENUM:
            return "AL_INVALID_ENUM";
        break;

        case AL_INVALID_VALUE:
            return "AL_INVALID_VALUE";
        break;

        case AL_INVALID_OPERATION:
            return "AL_INVALID_OPERATION";
        break;

        case AL_OUT_OF_MEMORY:
            return "AL_OUT_OF_MEMORY";
        break;
    };
    
    return "unkown error code";
}



Sound * Sound::ptr = NULL;

Sound::Sound()
{
    if(ptr == NULL)
        ptr = this;
    
    //Initialize audio device
    
    device = alcOpenDevice(NULL);   //selects default device
    if(device)
    {
        context = alcCreateContext(device, NULL);
        alcMakeContextCurrent(context);
    }else
    {
        LOG("Could not open audio device!", "audio");
        exit(2);
    }
    
    alGetError();
    loadAudio("data/audio/engine.wav");
    if(alGetError() != AL_NO_ERROR)
    {
        LOG("Could not load audio", "audio");
        exit(2);
    }
    
   //setup the doppler effects
   alDopplerFactor(0.1);
    
}

void Sound::changePitch(unsigned int alSource, float delta)
{
    float t;
    alGetSourcef(alSource, AL_PITCH, &t);
    delta += t;
    
    if(delta > 5)
    {
        delta = 5;
    }
    
    if(delta < 1)
    {
        delta = 1;
    }
    alSourcef(alSource, AL_PITCH, delta);
}

Sound * const Sound::GetInstance()
{
    return ptr;
}

void Sound::setListener(btVector3 const & pos, btVector3 const & vel, float * orientation)
{
    alListener3f(AL_POSITION, pos.x(), pos.y(), pos.z() );
    alListener3f(AL_VELOCITY, vel.x(), vel.y(), vel.z() );
    alListenerfv(AL_ORIENTATION, orientation);
    
    LOG("listener: " << pos, "audio");
    
}

void Sound::setSource(unsigned int alSource, btVector3 const & pos, btVector3 const & vel, btVector3 const & dir)
{
    alSource3f(alSource, AL_POSITION, pos.x(), pos.y(), pos.z());
    alSource3f(alSource, AL_VELOCITY, vel.x(), vel.y(), vel.z());
    alSource3f(alSource, AL_DIRECTION, dir.x(), dir.y(), dir.z());
    LOG("source: " << pos, "audio");
}

void Sound::playAllSources()
{
    alGetError();
    for(Sources::iterator itr = sources.begin(); itr != sources.end(); ++itr)
    {
        alSourceStop(*itr);
        alSourceRewind(*itr);
        alSourcePlay(*itr);
        LOG("alSource " << (*itr), "audio");
    }
    
    LOG("error " << GetALErrorString(alGetError()), "audio");
    
    LOG("PLAY AUDIO", "audio");
}

void Sound::loadAudio(char * file)
{
    char * alBuffer;
    data.push_back(alBuffer);    //add to our audio library
    ALenum alFormatBuffer;  //buffer format
    ALsizei alFreqBuffer;   //buffer frequency
    ALsizei alBufferLen;       //bitdepth
    ALboolean alLoop;       //looped
    
    //load the wav file
    #if defined(__APPLE__)
    alutLoadWAVFile(file, &alFormatBuffer, (void **) &data.back(), &alBufferLen, &alFreqBuffer);
    #else
    alutLoadWAVFile(file, &alFormatBuffer, (void **) &data.back(), &alBufferLen, &alFreqBuffer, &alLoop);
    #endif
    
    unsigned int alSampleSet;
    //create buffer
    alGenBuffers(1, &alSampleSet);
    
    //add to buffers
    buffers.push_back(alSampleSet);
    //add to the key map for others to use
    keys.insert( DataKeyPair( file, buffers.size() -1 ) );	
        
    //fill sample
    alBufferData(alSampleSet, alFormatBuffer, data.back(), alBufferLen, alFreqBuffer);    
    
    
    //lazy for now all loop
    loops.push_back(AL_TRUE);
    
}

unsigned int Sound::addSource(char * file)
{
    //testing with temp source
    unsigned int alSource;
    
    //create source
    alGenSources(1, &alSource);
    
    //connect source to buffer
    alSourcei(alSource, AL_BUFFER, buffers[keys[file]]);
    
    //tell the sound to loop continuously
    alSourcei(alSource,AL_LOOPING,loops[keys[file]]);
    
    alSource3f(alSource, AL_POSITION, 0,0,0);
    alSource3f(alSource, AL_VELOCITY, 0,0,0);
    
    //how far it can go
    alSourcef(alSource, AL_REFERENCE_DISTANCE, 50);
    
    
    //add to sources
    sources.push_back(alSource);
    
    
    return alSource;
}

Sound::~Sound()
{


}
