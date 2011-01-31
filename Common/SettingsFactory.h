#ifndef SETTINGSFACTORY_H
#define SETTINGSFACTORY_H

#include "Tinyxml/tinyxml.h"
#include <map>

class SettingsFactory
{
public:
	static TiXmlDocument * loadSettings(char const * filename);
private:
	class HashVisitor : public TiXmlVisitor
	{
	public:
		HashVisitor(std::map<std::string, std::string> * const);
	private:
		bool VisitEnter(TiXmlElement const &, TiXmlAttribute const *);
		
		std::map<std::string, std::string> * settings;
	};
};

#endif