#pragma once

#include "pushbutton.h"

class QPaintEvent;
class CDockWidgetTitleBar;

class CTitleBarButton : public CPushButton
{
public:
	CTitleBarButton(CDockWidgetTitleBar* pParent = nullptr);
	void DefaultPaintEvent(QPaintEvent* pEvent) { CPushButton::paintEvent(pEvent); }

private:
	void paintEvent(QPaintEvent* pEvent);
	
	CDockWidgetTitleBar* m_pParent;
};

