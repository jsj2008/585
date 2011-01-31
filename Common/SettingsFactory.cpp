#include "SettingsFactory.h"
#include <stdlib.h>
#include <iostream>

SettingsFactory * SettingsFactory::ptr = NULL;

SettingsFactory::SettingsFactory()
{
	if(ptr == NULL)
		ptr = this;	//singleton
}

Settings SettingsFactory::loadSettings(char const * filename)
{
	if(ptr->all_settings.find(filename) != ptr->all_settings.end() )	//already loaded
	{
		return ptr->all_settings[filename];
	}
	
	TiXmlDocument doc(filename);
	bool loadOk = doc.LoadFile();
	
	if(loadOk)
	{
		
		ptr->all_settings.insert(AllSettingsPair(filename, Settings() ));
		HashVisitor visitor(& (ptr->all_settings[filename]) );
		doc.Accept(&visitor);
		
		return (ptr->all_settings[filename]);	
		
	}
	else
	{
		std::cout << "failed to load settings:" << filename << std::endl;
		std::cout << "Check runtime path! Check XML is parsed correctly!" << std::endl;
	}
	
	/*find a way to return NULL*/
}

SettingsFactory::HashVisitor::HashVisitor(Settings * const settings) : settings(settings) {}

bool SettingsFactory::HashVisitor::VisitEnter(TiXmlElement const & , TiXmlAttribute const * att)
{
	while(att != NULL)
	{
		settings->insert( SettingsPair(att->Name(), att->Value() ) );		
		att = att->Next();
	}
	return true;
}