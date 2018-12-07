#include "dockwidget.h"
#include <settings.h>

#include "titlebar.h"
#include <QPaintEvent>
#include <QPainter>

CDockWidget::CDockWidget(const QString& strTitle, QWidget* pParent, Qt::WindowFlags eFlags)
	: QDockWidget(strTitle, pParent, eFlags)
	, m_pTitleBar(new CDockWidgetTitleBar(strTitle, this))
	, m_bFocused(false)
{
	SetupUI();
}

CDockWidget::CDockWidget(QWidget * pParent, Qt::WindowFlags eFlags)
	: QDockWidget(pParent, eFlags)
{
	SetupUI();
}

void CDockWidget::SetupUI()
{
	setTitleBarWidget(m_pTitleBar);
	connect(m_pTitleBar, SIGNAL(SigClose()), this, SLOT(close()));
	connect(m_pTitleBar, SIGNAL(SigClicked()), this, SLOT(SetFocus()));
}

void CDockWidget::paintEvent(QPaintEvent* pEvent)
{
	QPainter ptr(this);
	ptr.setPen(QPen(CSettings::GetGlobalDefaultSettings()->dockwidget.backgroundfocus, 2));

	if (widget()->hasFocus() != m_bFocused)
	{
		m_bFocused = widget()->hasFocus();
		emit SigFocusChanged(m_bFocused);
	}

	if(Focused())
		ptr.setBrush(CSettings::GetGlobalDefaultSettings()->dockwidget.backgroundfocus);
	else
		ptr.setBrush(CSettings::GetGlobalDefaultSettings()->dockwidget.background);

	ptr.drawRect(pEvent->rect());
}

void CDockWidget::SetFocus()
{
	widget()->setFocus();
}

void CDockWidget::closeEvent(QCloseEvent* pEvent)
{
	emit SigClosed();
}