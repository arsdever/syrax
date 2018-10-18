#include "mousestat.h"

#include <QEvent>
#include <QMouseEvent>

IMouseStatements::IMouseStatements(QWidget* pParent)
	: QWidget(pParent)
	, m_bHover(false)
	, m_bPressed(false)
{
}

void IMouseStatements::enterEvent(QEvent* pEvent)
{
	m_bHover = true;
	emit SigHoverStateChanged(m_bHover);
	update();
	QWidget::enterEvent(pEvent);
}

void IMouseStatements::leaveEvent(QEvent* pEvent)
{
	m_bHover = false;
	emit SigHoverStateChanged(m_bHover);
	update();
	QWidget::leaveEvent(pEvent);
}

void IMouseStatements::mousePressEvent(QMouseEvent* pEvent)
{
	m_bPressed = true;
	emit SigPressStateChanged(m_bPressed);
	update();
	QWidget::mousePressEvent(pEvent);
}

void IMouseStatements::mouseReleaseEvent(QMouseEvent* pEvent)
{
	m_bPressed = false;
	emit SigPressStateChanged(m_bPressed);
	update();

	if (rect().contains(pEvent->pos()))
		emit SigClicked();

	QWidget::mouseReleaseEvent(pEvent);
}
