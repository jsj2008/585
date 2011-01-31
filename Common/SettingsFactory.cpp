#include "SettingsFactory.h"
#include <stdlib.h>
#include <iostream>

SettingsFactory * SettingsFactory::ptr = NULL;

SettingsFactory::SettingsFactory()
{
	if(ptr != NULL)
		return;	//singleton
	
	ptr = this;
	
	/*find all files to load*/
	char const * filename = "config/example1.xml";
	TiXmlDocument doc(filename);	//this is hardcoded for now
	bool loadOk = doc.LoadFile();
	
	if(loadOk)
	{
		Settings * settings = new Settings();
		ptr->all_settings.insert(AllSettingsPair(filename, settings ));
		// HashVisitor visitor(settings);
		 // doc.Accept(&visitor);
		
	}
	else
	{
		std::cout << "failed to load settings:" << filename << std::endl;
		std::cout << "Check runtime path! Check XML is parsed correctly!" << std::endl;
	}
		
		
}

/*Settings const * SettingsFactory::loadSettings(char const * filename)
{
	if(ptr->all_settings.find(filename) != ptr->all_settings.end() )	//already loaded
	{
		return ptr->all_settings[filename];
	}
	
	TiXmlDocument doc(filename);
	bool loadOk = doc.LoadFile();
	
	if(loadOk)
	{
		Settings * settings = new Settings();
				ptr->all_settings.insert(AllSettingsPair(filename, settings ));
				HashVisitor visitor(settings );
				 doc.Accept(&visitor);
				
				return settings;
		
	}
	else
	{
		std::cout << "failed to load settings:" << filename << std::endl;
		std::cout << "Check runtime path! Check XML is parsed correctly!" << std::endl;
	}
	
	/*find a way to return NULL*/
//}

SettingsFactory::HashVisitor::HashVisitor(Settings * const settings) : settings(settings) {}

bool SettingsFactory::HashVisitor::VisitEnter(TiXmlElement const & elem , TiXmlAttribute const * att)
{
	void * val;
	if("float".compare(att->Value() ) == 0)
	{
		val = new float(atof(elem.GetText() ));
	}
	settings->insert( SettingsPair(att->Name(), val ) );		
	
	return true;
}

SettingsFactory::~SettingsFactory()
{
	for(AllSettings::iterator itr=ptr->all_settings.begin(); itr != ptr->all_settings.end(); ++ itr)
		{
			delete itr->second;
		}
}