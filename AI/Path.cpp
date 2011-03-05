#include "Path.h"
#include "Physics/HeightMapManager.h"

Path::Path() {
	testCube = RenderObject("blank.bmp", "blank.bmp", "models/cube.obj");
	hm = HeightMapManager::GetHeightMap();
	

	xscale = LoadFloat("config/world.xml","height_map_scale_x");		// Load the scaling information
	yscale = LoadFloat("config/world.xml","height_map_scale_y");
	zscale = LoadFloat("config/world.xml","height_map_scale_z");

	addPoint(318, 8);
	addPoint(320, 38);
	addPoint(308, 62);
	addPoint(297, 77);
	addPoint(305, 93);
	addPoint(322, 94);
	addPoint(351, 90);
	addPoint(383, 86);
	addPoint(409, 89);
	addPoint(427, 103);
	addPoint(446, 117);
	addPoint(466, 129);
	addPoint(480, 148);
	addPoint(477, 172);
	addPoint(469, 200);
	addPoint(458, 225);
	addPoint(442, 225);
	addPoint(421, 208);
	addPoint(401, 186);
	addPoint(363, 175);
	addPoint(312, 165);
	addPoint(255, 159);
	addPoint(214, 170);
	addPoint(183, 179);
	addPoint(138, 164);
	addPoint(117, 150);
	addPoint(100, 113);
	addPoint(79, 97);
	addPoint(63, 96);
	addPoint(51, 115);
	addPoint(41, 163);
	addPoint(45, 200);
	addPoint(56, 241);
	addPoint(62, 286);
	addPoint(65, 327);
	addPoint(64, 346);
	addPoint(58, 367);
	addPoint(61, 393);
	addPoint(70, 420);
	addPoint(77, 444);
	addPoint(87, 460);
	addPoint(106, 464);
	addPoint(122, 458);
	addPoint(136, 441);
	addPoint(156, 418);
	addPoint(173, 396);
	addPoint(192, 385);
	addPoint(209, 389);
	addPoint(225, 412);
	addPoint(234, 468);
	addPoint(212, 496);
}

Path::~Path() { }

void Path::addPoint(int x, int z) {
	double xd = (double)x/2.0; // hack
	double zd = (double)z/2.0;
	points.push_back(Point((xd) * xscale, (float)(hm->map[((int)zd*hm->width)+(int)xd]) * yscale + 3, (zd) * zscale));
}

void Path::debugDraw(Point playerPos) {
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
		glTranslated(playerPos.x, playerPos.y, playerPos.z);
		testCube.draw();
	glPopMatrix();
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
