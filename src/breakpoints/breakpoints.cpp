#include "breakpoints.h"
#include <QMenuBar>

CBreakpoints::CBreakpoints()
{
}

extern "C" BREAKPOINTS_EXPORT void LoadPlugin(QMenuBar* pMenuBar)
{
	QList<QMenu*> menus = pMenuBar->findChildren<QMenu*>();
	QMenu* viewMenu = nullptr;
	for (QMenu* menu : menus)
	{
		if (menu->title() != "View")
			continue;

		viewMenu = menu;
		break;
	}

	if (viewMenu == nullptr)
		viewMenu = pMenuBar->addMenu("View");

	viewMenu->addAction("Breakpoints");
	QMenu* pMenu = pMenuBar->addMenu("Breakpoints");
	pMenu->addAction("Set breakpoint");
	pMenu->addAction("Clear breakpoints");
}