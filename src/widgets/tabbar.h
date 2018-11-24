#pragma once

#include <QTabBar>

class QPaintEvent;

class CTabBar : public QTabBar
{
public:
	CTabBar(QWidget* pParent = 0);
	~CTabBar();

protected:
	void paintEvent(QPaintEvent* pEvent);
};