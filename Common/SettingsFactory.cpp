#include "SettingsFactory.h"
#include <stdlib.h>
#include <iostream>

SettingsFactory * SettingsFactory::ptr = NULL;

void SettingsFactory::reload()
{
	for(Settings::iterator itr = ptr->all_settings["config/files.xml"]->begin(); itr != ptr->all_settings["config/files.xml"]->end(); ++itr)
	{
		ptr->loadSettings( *(static_cast<std::string *>(itr->second)) );	//loads each file
	}
}

void SettingsFactory::loadSettings(std::string const & filename)
{
	TiXmlDocument doc( filename.c_str() );	//this is hardcoded for now
	bool loadOk = doc.LoadFile();

	if(loadOk)
	{
		if(ptr->all_settings.find(filename) == ptr->all_settings.end() )	//new settings
		{
			Settings * settings = new Settings();
			ptr->all_settings.insert(AllSettingsPair(filename, settings ));
			HashVisitor visitor(settings);
			doc.Accept(&visitor);
			
		}else	//reloading
		{
			HashVisitor visitor(ptr->all_settings[filename]);
			doc.Accept(&visitor);
		}
	
	}
	else
	{
		std::cout << "failed to load settings:" << filename << std::endl;
		std::cout << "Check runtime path! Check XML is parsed correctly!" << std::endl;
	}
}

SettingsFactory::SettingsFactory()
{
	if(ptr != NULL)
		return;	//singleton
	
	ptr = this;
	/*find all files to load*/
	loadSettings("config/files.xml");
	SettingsFactory::reload();
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
			if(settings->find(elem.Attribute("name")) == settings->end())
			{
				float * f = new float();
				elem.QueryFloatAttribute("value", f );
				val = static_cast<void *>(f);
				break;
			}else
			{
				
				elem.QueryFloatAttribute( "value", static_cast<float *>((*settings)[elem.Attribute("name")]) );
				return true;
				
			}
		}
		case TYPE_INT:
		{
			if(settings->find(elem.Attribute("name")) == settings->end())
			{
				int * i = new int();
				elem.QueryIntAttribute("value", i );
				val = static_cast<void *>(i);
				break;
			}else
			{
				std::cout << elem.Attribute("name") << std::endl;
				elem.QueryIntAttribute( "value", static_cast<int *>((*settings)[elem.Attribute("name")]) );
				return true;
			}
		}
		case TYPE_DOUBLE: 
		{
			if(settings->find(elem.Attribute("name")) == settings->end())
			{
				double * d = new double();
				elem.QueryDoubleAttribute("value", d );
				val = static_cast<void *>(d);
				break;	
			}else
			{
				elem.QueryDoubleAttribute( "value", static_cast<double *>((*settings)[elem.Attribute("name")]) );
				return true;
			}
		}
		case TYPE_STRING: 
		{
			if(settings->find(elem.Attribute("name")) == settings->end())
			{
				std::string * s = new std::string(elem.Attribute("value") );
				val = static_cast<void *>(s);
				break;	
			}	//doesn't allow for reloading strings
		}
	}

	settings->insert( SettingsPair(elem.Attribute("name"), val ) );		
	
	return true;
}

SettingsFactory::~SettingsFactory()
{
	for(AllSettings::iterator itr=ptr->all_settings.begin(); itr != ptr->all_settings.end(); ++ itr)
		{
			Settings * settings = itr->second;
			for(Settings::iterator itr2 = settings->begin(); itr2 != settings->end(); ++ itr2)
			{
				delete itr2->second;
			}
			delete itr->second;
		}
}