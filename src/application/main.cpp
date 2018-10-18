#include "application.h"
#include <QApplication>
#include <QFontDatabase>
#include <core.h>
#include "../console/console_mgr.h"

int main(int argc, char *argv[])
{
	new CBreakpointManager(GetCore());
	new CConsoleManager(GetCore());
	QApplication a(argc, argv);
	int id = QFontDatabase::addApplicationFont(":/fonts/Resources/UbuntuMono-Regular.ttf");
	CApplication w;
	w.show();
	return a.exec();
}
