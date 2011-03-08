#include "HeightMap.h"
#include "HeightMapManager.h"
#include "Common/SettingsFactory.h"

HeightMapManager * HeightMapManager::ptr = NULL;

HeightMapManager::HeightMapManager() : h( new HeightMap(LoadString2("config/world.xml","height_map")))
{
	if(ptr == NULL)
	{
		ptr = this;
	}
}

HeightMap * HeightMapManager::GetHeightMap()
{
	return ptr->getH();
}