#include "SettingsFactory.h"
#include <stdlib.h>
#include <iostream>

TiXmlDocument * SettingsFactory::loadSettings(char const * filename)
{
	TiXmlDocument doc(filename);
	bool loadOk = doc.LoadFile();
	
	if(loadOk)
	{
		std::map<std::string, std::string> * settings = new std::map<std::string, std::string>();
		
		HashVisitor visitor(settings);
		
		doc.Accept(&visitor);	
		std::cout << settings->at("test") << std::endl;
		
	}
	else
	{
		std::cout << "failed to load settings:" << filename << std::endl;
	}
			std::cout <<"A" << std::endl;
	
	return NULL;
}

SettingsFactory::HashVisitor::HashVisitor(std::map<std::string, std::string> * const settings) : settings(settings) {}

bool SettingsFactory::HashVisitor::VisitEnter(TiXmlElement const & , TiXmlAttribute const * att)
{
	while(att != NULL)
	{
		settings->insert( std::pair<std::string, std::string>(att->Name(), att->Value() ) );		
		att = att->Next();
	}
	return true;
}