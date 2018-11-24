#include "tabbar.h"
#include <QAction>

CTabBar::CTabBar(QWidget* pParent)
	:QTabBar(pParent)
{
	addAction(new QAction("Close"));
}

CTabBar::~CTabBar()
{
}

void CTabBar::paintEvent(QPaintEvent* pEvent)
{
	QTabBar::paintEvent(pEvent);
}