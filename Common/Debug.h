#ifndef DEBUG_H
#define DEBUG_H
#include <btBulletDynamicsCommon.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Common/SettingsFactory.h"
std::ostream & operator<<(std::ostream &, btVector3 const &);
class Debug
{
public:
	static void log(std::string const & data, std::string const & type);
	static void OpenFile();
	static void CloseFile();

private:
	static std::fstream file;
};

class anon_stream
{
public:
	operator std::string() const
	{
		return out.str();
	}
	
	template <typename T>
	anon_stream & operator<<(T const & t)
	{
		out << t;
		return *this;
	}
	
private:
	std::ostringstream out;
};

#ifndef RELEASE
#define LOG(a,b) Debug::log(anon_stream() << a, b)
#define START_LOG() Debug::OpenFile()
#define END_LOG() Debug::CloseFile()
#else
#define LOG(a,b)
#define START_LOG
#define END_LOG()
#endif

/*template<typename T> 
std::string & operator<<(std::cout & a, T)
{
	return a;
}*/


#endif

