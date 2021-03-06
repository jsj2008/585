#include "Main/MainController.h"
#include <iostream>
#include "Common/SettingsFactory.h"
#include "Common/Debug.h"
#include "Physics/HeightMapManager.h"

int main(int , char **) {
	/*setup settings singleton*/
	SettingsFactory settings_singleton;
	START_LOG();
	HeightMapManager hm;
	MainController m;
	m.window.stopLoading();
	m.window.run(&m);
	END_LOG();
	return 0;
}
