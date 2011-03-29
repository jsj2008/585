#ifndef SOUND_H
#define SOUND_H
#include <sound_dependencies/alc.h>
#include <sound_dependencies/al.h>
#include <sound_dependencies/alut.h>
#include <btBulletDynamicsCommon.h>
#include <vector>
#include <map>

class Sound
{
	public:

	Sound();
	~Sound();
	
    
    static Sound * ptr;
    static Sound * const GetInstance();
    void setListener(btVector3 const & pos, btVector3 const & vel, float * orientation);
    unsigned int addSource(char *, bool relative = false);   //used to register new source from jeeps and such
    void playAllSources();
    void pauseAllSources();
    void restartAllSources();
    void playAllDynamicSources();
    void pauseAllDynamicSources();
    void restartAllDynamicSources();
    void setSource(unsigned int, btVector3 const & pos, btVector3 const & vel, btVector3 const & dir);
    void changeGain(unsigned int, float);
    void setPitch(unsigned int, float);
    void playSource(unsigned int);
    void setAndPlaySource(unsigned int, btVector3 const &);
    void pauseSource(unsigned int);
    void restartSource(unsigned int);
    void rewindSource(unsigned int);
    
protected:
    ALCcontext * context;
    ALCdevice * device;
    
    typedef std::vector<char *> Data;
    typedef std::vector<unsigned int> Buffers;
    typedef std::vector<unsigned int> Sources;
    typedef std::map<char *, unsigned int> DataKey;
    typedef std::pair<char *, unsigned int> DataKeyPair;
    typedef std::vector<int> Loops;
    Data data;
    Buffers buffers;
    Sources sources;
    Sources dSources;
    DataKey keys;
    Loops loops;
    
    void loadAudio(char *, bool loop = true);

    

};
#endif