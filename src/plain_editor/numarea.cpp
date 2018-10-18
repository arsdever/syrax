#include "numarea.h"

#include <decorations.h>

#include <QPainter>
#include <QPaintEvent>

CNumberArea::CNumberArea(CLineNumberDecoration* pParent)
	: QWidget(pParent)
{}

void CNumberArea::paintEvent(QPaintEvent* pEvent)
{
	dynamic_cast<CLineNumberDecoration*>(parent())->DrawNumberArea(pEvent);
}