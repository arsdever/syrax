#include "decor_breakpoint.h"
#include "bparea.h"

#include <settings.h>
#include <QTextBlock>
#include <manipulators.h>
#include <QResizeEvent>
#include <QPainter>

CBreakpointDecoration::CBreakpointDecoration(QPlainTextEdit* pCore)
	: IPlainTextEditDecorator(pCore)
	, m_pBreakpointArea(new CBreakpointArea(this))
	, m_nLocalWidth(0)
{
	UpdateBreakpointAreaWidth();
}

void CBreakpointDecoration::UpdateBreakpointAreaWidth()
{
	int wdt = BreakpointAreaWidth();
	QMargins mgs = viewportMargins() + QMargins(wdt, 0, 0, 0) - QMargins(m_nLocalWidth, 0, 0, 0);
	m_nLocalWidth = wdt;
	setViewportMargins(mgs);

	QRect cr = contentsRect();
	m_pBreakpointArea->setGeometry(QRect(cr.left(), cr.top(), wdt, cr.height()));
}

void CBreakpointDecoration::UpdateBreakpointArea(QRect rect, int dy)
{
	if (dy)
		m_pBreakpointArea->scroll(0, dy);
	else
		m_pBreakpointArea->update(0, rect.y(), m_pBreakpointArea->width(), rect.height());
}

int CBreakpointDecoration::BreakpointAreaWidth()
{
	return fontMetrics().lineSpacing();
}

void CBreakpointDecoration::paintEvent(QPaintEvent* pEvent)
{
	QRect dr = pEvent->rect();
	IPlainTextEditDecorator::paintEvent(pEvent);
}

void CBreakpointDecoration::DrawBreakpointArea(QPaintEvent* pEvent)
{
	QPainter painter(m_pBreakpointArea);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.fillRect(pEvent->rect(), CSettings::GetGlobalDefaultSettings()->editor.breakpointarea.background);
	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int)blockBoundingRect(block).height();
	int size = BreakpointAreaWidth();
	painter.setBrush(CSettings::GetGlobalDefaultSettings()->editor.breakpointarea.breakpoint.background);
	painter.setPen(QPen(CSettings::GetGlobalDefaultSettings()->editor.breakpointarea.breakpoint.bordercolor, CSettings::GetGlobalDefaultSettings()->editor.breakpointarea.breakpoint.borderwidth));
	while (block.isValid() && top <= pEvent->rect().bottom())
	{
		if (block.isVisible() && bottom >= pEvent->rect().top() && m_pBreakpointArea->GetBreakpointList().contains(blockNumber + 1))
		{
			painter.drawEllipse(3, top + 3, size - 6, size - 6);
		}

		//if (m_nExecutionLine == blockNumber)
		//{
		//	QPolygon* playIcon = new QPolygon();
		//	playIcon->putPoints(0, 3, 5, top + 5, 5, top + size - 5, size - 5, top + size / 2);
		//	painter.save();
		//	painter.setBrush(CSettings::GetGlobalDefaultSettings()->editor.breakpointarea.runingpoint.background);
		//	painter.setPen(QPen(CSettings::GetGlobalDefaultSettings()->editor.breakpointarea.runingpoint.bordercolor, CSettings::GetGlobalDefaultSettings()->editor.breakpointarea.runingpoint.borderwidth));
		//	painter.drawPolygon(*playIcon);
		//	painter.restore();
		//}

		block = block.next();
		top = bottom;
		bottom = top + (int)blockBoundingRect(block).height();
		++blockNumber;
	}
}

ui32 CBreakpointDecoration::GetLineIndexByCoordinate(ui32 nYPos)
{
	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int)blockBoundingRect(block).height();
	while (block.isValid())
	{
		if (nYPos <= bottom && nYPos >= top)
		{
			return blockNumber;
		}

		block = block.next();
		top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
		bottom = top + (int)blockBoundingRect(block).height();
		++blockNumber;
	}

	return -1;
}

void CBreakpointDecoration::resizeEvent(QResizeEvent* pEvent)
{
	QPlainTextEdit::resizeEvent(pEvent);
	UpdateBreakpointAreaWidth();
}