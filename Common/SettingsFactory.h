#ifndef SETTINGSFACTORY_H
#define SETTINGSFACTORY_H

#include "Tinyxml/tinyxml.h"
#include <map>

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
	template<class T> static T const * get(std::string const & filename, std::string const & key)
	{
		Settings const * settings = ptr->all_settings[filename];
		return static_cast<T *> ( settings->at(key) );
	}
	
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