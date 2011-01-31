#include "Main/MainController.h"
#include <iostream>
#include "Common/SettingsFactory.h"

int main(int , char **) {

	SettingsFactory::loadSettings("config/example1.xml");
	//MainController m;
	return 0;
}
