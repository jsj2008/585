#include "Main/MainController.h"
#include <iostream>
#include "Common/SettingsFactory.h"
#include "Common/Debug.h"

int main(int , char **) {
	/*setup settings singleton*/
	SettingsFactory settings_singleton;
	START_LOG();
	MainController m;
	m.window.run(&m);
	END_LOG();
	return 0;
}
