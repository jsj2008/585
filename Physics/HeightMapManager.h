#ifndef HEIGHTMAPMANGER_H
#define HEIGHTMAPMANAGER_H

class HeightMap;

class HeightMapManager
{
public:
	HeightMapManager();
	static HeightMap * GetHeightMap();	
	static HeightMapManager * ptr;
	HeightMap * getH(){return h;}
private:
	HeightMap * h;
};

#endif