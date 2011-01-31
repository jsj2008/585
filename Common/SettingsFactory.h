#ifndef SETTINGSFACTORY_H
#define SETTINGSFACTORY_H

#include "Tinyxml/tinyxml.h"
#include <map>

typedef std::map<std::string const, std::string const> Settings;
typedef std::pair<std::string const, std::string const> SettingsPair;
typedef std::pair<std::string const, Settings const> AllSettingsPair;

class SettingsFactory
{
public:
	static Settings loadSettings(char const * filename);
	SettingsFactory();
	
private:
	

	
	static SettingsFactory * ptr;
	std::map<std::string, Settings const> all_settings;

	
	class HashVisitor : public TiXmlVisitor
	{
	public:
		HashVisitor(Settings * const);
	private:
		bool VisitEnter(TiXmlElement const &, TiXmlAttribute const *);
		std::map<std::string const, std::string const> * settings;
	};
};

#endif