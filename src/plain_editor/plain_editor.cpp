#include "plain_editor.h"

#include "numarea.h"
#include "bparea.h"
#include "settings.h"
#include <manipulators.h>

#include <QPlainTextEdit>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QTextBlock>
#include <QDebug>

CPlainEditor::CPlainEditor(QWidget * pParent)
	: QPlainTextEdit(pParent)
	, m_nExecutionLine(-1)
{
	SetupUI();
}

void CPlainEditor::SetupUI()
{
	m_pBreakpointArea = new CBreakpointArea(this);

	setTabStopWidth(4 * fontMetrics().width(QLatin1Char('9')));
	setAutoFillBackground(true);

	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	highlightCurrentLine();
}

// ---------------------------------------------------------------------------------------------------------------------

int CPlainEditor::BreakpointAreaWidth()
{
	return fontMetrics().lineSpacing();
}

void CPlainEditor::updateLineNumberArea(const QRect &rect, int dy)
{
	if (dy)
	{
		m_pBreakpointArea->scroll(0, dy);
	}
	else
	{
		m_pBreakpointArea->update(0, rect.y(), m_pBreakpointArea->width(), rect.height());
	}
}

void CPlainEditor::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	QRect cr = contentsRect();
	m_pBreakpointArea->setGeometry(QRect(cr.left(), cr.top(), BreakpointAreaWidth(), cr.height()));
}

void CPlainEditor::highlightCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!textCursor().hasSelection())
	{
		QTextEdit::ExtraSelection selection;
		selection.format.setProperty(QTextFormat::BlockFormat, true);
		selection.format.setBackground(CSettings::GetGlobalDefaultSettings()->editor.currentline.background);
		selection.format.setForeground(CSettings::GetGlobalDefaultSettings()->editor.currentline.foreground);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.format.setProperty(QTextFormat::OutlinePen, QPen(CSettings::GetGlobalDefaultSettings()->editor.currentline.border, CSettings::GetGlobalDefaultSettings()->editor.currentline.borderwidth));
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}

void CPlainEditor::DrawBreakpointArea(QPaintEvent* pEvent)
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

		if (m_nExecutionLine == blockNumber)
		{
			QPolygon* playIcon = new QPolygon();
			playIcon->putPoints(0, 3, 5, top + 5, 5, top + size - 5, size - 5, top + size / 2);
			painter.save();
			painter.setBrush(CSettings::GetGlobalDefaultSettings()->editor.breakpointarea.runingpoint.background);
			painter.setPen(QPen(CSettings::GetGlobalDefaultSettings()->editor.breakpointarea.runingpoint.bordercolor, CSettings::GetGlobalDefaultSettings()->editor.breakpointarea.runingpoint.borderwidth));
			painter.drawPolygon(*playIcon);
			painter.restore();
		}

		block = block.next();
		top = bottom;
		bottom = top + (int)blockBoundingRect(block).height();
		++blockNumber;
	}
}

int CPlainEditor::GetLineIndexByCoordinate(int nYPos)
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

ui32 CPlainEditor::CurrentLineNumber()
{
	return textCursor().blockNumber() + 1;
}

//void CPlainEditor::SetBreakpointEvent(QMouseEvent* pEvent)
//{
//	QTextBlock block = firstVisibleBlock();
//	int blockNumber = block.blockNumber();
//	int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
//	int bottom = top + (int)blockBoundingRect(block).height();
//	while (block.isValid())
//	{
//		if (pEvent->pos().y() <= bottom && pEvent->pos().y() >= top)
//		{
//			if (m_pBreakpointArea->GetBreakpointList().indexOf(blockNumber + 1) == -1)
//			{
//				OnBPSet(blockNumber + 1);
//				emit SigBPSet(blockNumber + 1);
//			}
//			else
//			{
//				OnBPUnset(blockNumber + 1);
//				emit SigBPUnset(blockNumber + 1);
//			}
//
//			m_pBreakpointArea->update();
//			return;
//		}
//
//		block = block.next();
//		top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
//		bottom = top + (int)blockBoundingRect(block).height();
//		++blockNumber;
//	}
//}
//
//void CPlainEditor::ToggleBreakpoint(int nLineNumber)
//{
//	if (nLineNumber <= 0)
//		nLineNumber = textCursor().blockNumber() + 1;
//
//	int index = m_pBreakpointArea->GetBreakpointList().indexOf(nLineNumber);
//	if (index == -1)
//	{
//		OnBPSet(nLineNumber);
//		emit SigBPSet(nLineNumber);
//	}
//	else
//	{
//		OnBPUnset(nLineNumber);
//		emit SigBPUnset(nLineNumber);
//	}
//}
//
//void CPlainEditor::OnBPSet(unsigned int nLineNumber)
//{
//	m_pBreakpointArea->GetBreakpointList().push_back(nLineNumber);
//	update();
//}
//
//void CPlainEditor::OnBPUnset(unsigned int nLineNumber)
//{
//	int index = m_pBreakpointArea->GetBreakpointList().indexOf(nLineNumber);
//	m_pBreakpointArea->GetBreakpointList().removeAt(index);
//	update();
//}
