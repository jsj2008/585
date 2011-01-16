#include "Main/MainController.h"
#include "Physics/Physics.h"
#include "Physics/libCube.h"
#include "Physics/libPlane.h"
#include <QtGui>
#include "Common/Actor.h"
#include "Renderer/Renderer.h"
#include "Renderer/GLDebugDrawer.h"
#include <iostream>

int main(int argc, char *argv[]) {

	QApplication a(argc, argv);
	MainController m;
	m.show();
	return a.exec();
}
