#include "Main/MainController.h"
#include <iostream>
#include "Common/SettingsFactory.h"

int main(int , char **) {
	/*setup settings singleton*/
	SettingsFactory settings_singleton;
	MainController m;
	return 0;
}
