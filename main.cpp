#include "Main/MainController.h"
#include "Common/SettingsFactory.h"
#include "Common/Debug.h"
#include "Physics/HeightMapManager.h"
#include "Renderer/Models.h"
#include "UI/Window.h"

int main(int , char **) {
	/*setup settings singleton*/
	SettingsFactory settings_singleton;
	START_LOG();
	HeightMapManager hm;
	
	Window window;	//launch SDL window
	MainController m(window);
	window.stopLoading();
	window.run(&m);
	END_LOG();
	return 0;
}
