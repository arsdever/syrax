#include "titlebarbutton.h"
#include "settings.h"
#include "icons.h"
#include "titlebar.h"

#include <QPaintEvent>
#include <QPainter>

CTitleBarButton::CTitleBarButton(CDockWidgetTitleBar* pParent)
	: CPushButton(pParent)
	, m_pParent(pParent)
{
}

void CTitleBarButton::paintEvent(QPaintEvent* pEvent) 
{
	if(m_pParent != nullptr)
		m_pParent->DrawButton(this, pEvent);
}