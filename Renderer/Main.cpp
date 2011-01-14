#include "Renderer.h"
#include <QtGui>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	Renderer r;
	r.show();
	return a.exec();
}
