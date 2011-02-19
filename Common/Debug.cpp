#include "Debug.h"

std::ostream & operator<<(std::ostream & s, btVector3 const & vec)
{
	return s << "(" << vec.x() << "," << vec.y() << "," << vec.z() << ")";
}

std::fstream Debug::file;

void Debug::log(std::string const & data, std::string const & type)
{
	if(LoadInt("config/debug.xml", type) == 1)
	{	
		if(LoadInt("config/debug.xml", "console") == 1)
			std::cout << data << std::endl;
		if(LoadInt("config/debug.xml", "file") == 1)
			file << data << std::endl;
			
	}
}



void Debug::OpenFile()
{
	Debug::file.open(LoadString2("config/debug.xml", "file_path").c_str(), std::fstream::out);
}

void Debug::CloseFile()
{
	Debug::file.close();
}