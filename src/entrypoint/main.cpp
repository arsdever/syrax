#include <QApplication>
#include <QFontDatabase>
#include <application.h>

typedef QWidget* (*FPTR)();

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	int id = QFontDatabase::addApplicationFont(":/fonts/Resources/UbuntuMono-Regular.ttf");
	CApplication w;
	w.show();
	return a.exec();
}
