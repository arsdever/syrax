#include "decor_breakpoint.h"
#include "bparea.h"

#include <settings.h>
#include <QTextBlock>
#include <manipulators.h>
#include <QResizeEvent>
#include <QPainter>

CBreakpointDecoration::CBreakpointDecoration(QAbstractScrollArea* pCore)
	: IPlainTextEditDecorator(pCore)
	//, m_pBreakpointArea(new CBreakpointArea(this))
	, m_nLocalWidth(0)
{
	UpdateBreakpointAreaWidth();
}

void CBreakpointDecoration::UpdateBreakpointAreaWidth()
{
	CPublicPlainTextEdit* editor = dynamic_cast<CPublicPlainTextEdit*>(CoreWidget());
	if (editor == nullptr)
		return;

	int wdt = BreakpointAreaWidth();
	QMargins mgs = editor->viewportMargins() + QMargins(wdt, 0, 0, 0) - QMargins(m_nLocalWidth, 0, 0, 0);
	m_nLocalWidth = wdt;
	editor->setViewportMargins(mgs);

	QRect cr = contentsRect();
	setGeometry(QRect(cr.left(), cr.top(), wdt, cr.height()));
}

void CBreakpointDecoration::UpdateBreakpointArea(QRect rect, int dy)
{
	if (dy)
		scroll(0, dy);
	else
		update(0, rect.y(), width(), rect.height());
}

int CBreakpointDecoration::BreakpointAreaWidth()
{
	return fontMetrics().lineSpacing();
}

void CBreakpointDecoration::paintEvent(QPaintEvent* pEvent)
{
	DrawBreakpointArea(pEvent);
	IPlainTextEditDecorator::paintEvent(pEvent);
}

void CBreakpointDecoration::DrawBreakpointArea(QPaintEvent* pEvent)
{
	CPublicPlainTextEdit* editor = dynamic_cast<CPublicPlainTextEdit*>(CoreWidget());
	if (editor == nullptr)
		return;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.fillRect(pEvent->rect(), CSettings::GetGlobalDefaultSettings()->editor.breakpointarea.background);
	QTextBlock block = editor->firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int)editor->blockBoundingGeometry(block).translated(editor->contentOffset()).top();
	int bottom = top + (int)editor->blockBoundingRect(block).height();
	int size = BreakpointAreaWidth();
	painter.setBrush(CSettings::GetGlobalDefaultSettings()->editor.breakpointarea.breakpoint.background);
	painter.setPen(QPen(CSettings::GetGlobalDefaultSettings()->editor.breakpointarea.breakpoint.bordercolor, CSettings::GetGlobalDefaultSettings()->editor.breakpointarea.breakpoint.borderwidth));
	while (block.isValid() && top <= pEvent->rect().bottom())
	{
		CBreakpointManager* pBPmanager = GetCore()->GetManager<CBreakpointManager>();
		if (pBPmanager == nullptr)
			return;

		if (block.isVisible() && bottom >= pEvent->rect().top() && pBPmanager->GetBreakpointList().contains(blockNumber + 1))
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
		bottom = top + (int)editor->blockBoundingRect(block).height();
		++blockNumber;
	}
}

ui32 CBreakpointDecoration::GetLineIndexByCoordinate(ui32 nYPos)
{
	CPublicPlainTextEdit* editor = dynamic_cast<CPublicPlainTextEdit*>(CoreWidget());
	if (editor == nullptr)
		return -1;

	QTextBlock block = editor->firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int)editor->blockBoundingGeometry(block).translated(editor->contentOffset()).top();
	int bottom = top + (int)editor->blockBoundingRect(block).height();
	while (block.isValid())
	{
		if (nYPos <= bottom && nYPos >= top)
		{
			return blockNumber;
		}

		block = block.next();
		top = (int)editor->blockBoundingGeometry(block).translated(editor->contentOffset()).top();
		bottom = top + (int)editor->blockBoundingRect(block).height();
		++blockNumber;
	}

	return -1;
}

void CBreakpointDecoration::resizeEvent(QResizeEvent* pEvent)
{
	QWidget::resizeEvent(pEvent);
	UpdateBreakpointAreaWidth();
}