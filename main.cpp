#include "Main/MainController.h"
#include <iostream>
#include "Common/SettingsFactory.h"

int main(int , char **) {
	SettingsFactory singleton;
	float const * k = SettingsFactory::getSettingPointer<float>("config/example1.xml", "c" );
	int const * i = SettingsFactory::getSettingPointer<int>("config/example1.xml", "test");
	std::cout << *i << std::endl;
	// Settings const * example1 = SettingsFactory::loadSettings("config/example1.xml");
	// double k;
	// k = strtod(example1->at("k").c_str());
	// std::cout << k*2 << std::endl;
	//MainController m;
	return 0;
}
