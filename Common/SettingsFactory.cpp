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
		HashVisitor visitor(settings);
		doc.Accept(&visitor);
		
	}
	else
	{
		std::cout << "failed to load settings:" << filename << std::endl;
		std::cout << "Check runtime path! Check XML is parsed correctly!" << std::endl;
	}
		
		
}

SettingsFactory::HashVisitor::HashVisitor(Settings * const settings) : settings(settings) {}

bool SettingsFactory::HashVisitor::VisitEnter(TiXmlElement const & elem , TiXmlAttribute const * )
{
	void * val;
	int type;
	elem.QueryIntAttribute("type", &type);
	switch(type)	//figure out which type
	{
		case TYPE_FLOAT:
		{ 
			float * f = new float();
			elem.QueryFloatAttribute("value", f );
			val = static_cast<void *>(f);
			break;
		}
		case TYPE_INT:
		{
			int * i = new int();
			elem.QueryIntAttribute("value", i );
			val = static_cast<void *>(i);
			break;
		}
		case TYPE_DOUBLE: 
		{
			double * d = new double();
			elem.QueryDoubleAttribute("value", d );
			val = static_cast<void *>(d);
			break;	
		}
	}

	settings->insert( SettingsPair(elem.Attribute("name"), val ) );		
	
	return true;
}

SettingsFactory::~SettingsFactory()
{
	for(AllSettings::iterator itr=ptr->all_settings.begin(); itr != ptr->all_settings.end(); ++ itr)
		{
			delete itr->second;
		}
}