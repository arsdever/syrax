#include "tabwidget.h"
#include "tabbar.h"

CTabWidget::CTabWidget(QWidget* pParent)
	: QTabWidget(pParent)
{
	QTabBar* pBar = new CTabBar();
	setTabBar(pBar);
}

CTabWidget::~CTabWidget()
{
}
