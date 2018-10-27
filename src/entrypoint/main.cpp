#include <application.h>
#include <core.h>
#include <bpmanager.h>
#include <editormgr.h>
#include <QApplication>
#include <QFontDatabase>
#include "../console/console_mgr.h"

int main(int argc, char *argv[])
{
	new CBreakpointManager(GetCore());
	new CConsoleManager(GetCore());
	new CApplicationManager(GetCore());
	new CEditorManager(GetCore());
	QApplication a(argc, argv);
	int id = QFontDatabase::addApplicationFont(":/fonts/Resources/UbuntuMono-Regular.ttf");
	CApplication w;
	w.show();
	return a.exec();
}
