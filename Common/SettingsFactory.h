#ifndef SETTINGSFACTORY_H
#define SETTINGSFACTORY_H

#include "Tinyxml/tinyxml.h"
#include <map>

typedef std::map<std::string const, void *> Settings;
typedef std::pair<std::string const, void*> SettingsPair;
typedef std::map<std::string const, Settings const *> AllSettings;
typedef std::pair<std::string const, Settings const *> AllSettingsPair;
typedef std::map<std::string const, std::string const *> XmlSettings;

class SettingsFactory
{
public:
	SettingsFactory();
	~SettingsFactory();
	
private:
	static SettingsFactory * ptr;
	AllSettings all_settings;

	
	class HashVisitor : public TiXmlVisitor
	{
	public:
		HashVisitor(Settings * const);
	private:
		bool VisitEnter(TiXmlElement const &, TiXmlAttribute const *);
		Settings * settings;
	};
};

#endif