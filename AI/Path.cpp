#include "Path.h"
#include "Physics/HeightMapManager.h"

Path::Path() { }

Path::Path(string filename) {
	testCube = RenderObject("blank.bmp", "blank.bmp", "models/cube.obj");
	hm = HeightMapManager::GetHeightMap();
	
	xscale = LoadFloat("config/world.xml","height_map_scale_x");		// Load the scaling information
	yscale = LoadFloat("config/world.xml","height_map_scale_y");
	zscale = LoadFloat("config/world.xml","height_map_scale_z");

	load(filename);
}

Path::~Path() { }

void Path::addPoint(int x, int z) {
	double xd = (double)x/2.0; // hack
	double zd = (double)z/2.0;
	points.push_back(Point((xd) * xscale, (float)(hm->map[((int)zd*hm->width)+(int)xd]) * yscale + 3, (zd) * zscale));
}

void Path::debugDraw(Point playerPathPos, Point playerPos) {
	Point point, nextPoint;
	for (int i = 0; i < points.size(); i++) {
		glColor3f(1,1,0);
		glDisable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(2);
		glPushMatrix();

		point = points.at(i);
			glTranslated(-((float)(hm->width*xscale))/2.0, 0, -((float)(hm->height*zscale))/2.0); // centering of map
			glTranslated(point.x, point.y, point.z);

			testCube.draw();
			if (i < points.size()-1) {
				nextPoint = points.at(i+1);
				glColor3f(1,1,1);
				glBegin(GL_LINES);
				glVertex3f(0,0,0);
				glVertex3f(nextPoint.x - point.x, nextPoint.y - point.y, nextPoint.z - point.z);
				glEnd();
			}

		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glColor3f(0,1,0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();
		glTranslated(-((float)(hm->width*xscale))/2.0, 0, -((float)(hm->height*zscale))/2.0); // centering of map
		glTranslated(playerPathPos.x, playerPathPos.y, playerPathPos.z);
		testCube.draw();
	glPopMatrix();
	glColor3f(0.2,1,0.5);
	glBegin(GL_LINES);
		glVertex3f(playerPos.x, playerPos.y, playerPos.z);
		glVertex3f(playerPathPos.x-((float)(hm->width*xscale))/2.0, playerPathPos.y, playerPathPos.z-((float)(hm->height*zscale))/2.0);
	glEnd();
	glLineWidth(1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
}

Point Path::at(int i) {
	return points.at(i);
}

int Path::length() {
	return points.size();
}

void Path::load(string filename) {
	points.clear();

	ifstream in;
	in.open(filename.c_str());

	double x, y;
	while (in >> x >> y)
		addPoint(x,y);
}
