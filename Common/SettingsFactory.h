#ifndef SETTINGSFACTORY_H
#define SETTINGSFACTORY_H

#define LoadFloat(a,b) SettingsFactory::get<float>(a,b)
#define LoadInt(a,b) SettingsFactory::get<int>(a,b)
#define LoadString2(a,b) SettingsFactory::get<std::string>(a,b)
#define LoadDouble(a,b) SettingsFactory::get<double>(a,b)

#include "Tinyxml/tinyxml.h"
#include <map>
#include <iostream>
#include <string>

typedef std::map<std::string const, void *> Settings;
typedef std::pair<std::string const, void*> SettingsPair;
typedef std::map<std::string const, Settings *> AllSettings;
typedef std::pair<std::string const, Settings *> AllSettingsPair;

class SettingsFactory
{
public:
	static const int TYPE_FLOAT = 0;
	static const int TYPE_INT = 1;
	static const int TYPE_DOUBLE = 2;
	static const int TYPE_STRING = 3;
	SettingsFactory();
	~SettingsFactory();
	template<class T> static T const & get(std::string const & filename, std::string const & key)
	{
		if(ptr->all_settings.find(filename) == ptr->all_settings.end())
		{
			std::cout << "could not load " << filename << std::endl;
		}
		Settings * settings = ptr->all_settings[filename];
		if(settings->find(key) == settings->end())
		{
			std::cout << "could not find property " << key << std::endl;
		}
		return *(static_cast<T *> ( (*settings)[key] ) );
	}
	
	static void reload();
	
private:
	static SettingsFactory * ptr;
	AllSettings all_settings;

	void loadSettings(std::string const & filename);
	
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