#include "decor_linenumber.h"
#include "numarea.h"

#include <settings.h>
#include <QTextBlock>
#include <manipulators.h>
#include <QResizeEvent>
#include <QPainter>

CLineNumberDecoration::CLineNumberDecoration(QPlainTextEdit* pCore)
	: IPlainTextEditDecorator(pCore)
	, m_pNumberArea(new CNumberArea(this))
	, m_nLocalWidth(0)
{
	UpdateNumberAreaWidth();
	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(UpdateNumberAreaWidth()));
	connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(UpdateNumberArea(QRect, int)));
}

void CLineNumberDecoration::UpdateNumberAreaWidth()
{
	int wdt = NumberAreaWidth();
	QMargins mgs = viewportMargins();
	mgs += QMargins(wdt, 0, 0, 0);
	mgs -= QMargins(m_nLocalWidth, 0, 0, 0);
	m_nLocalWidth = wdt;
	setViewportMargins(mgs);

	QRect cr = contentsRect();
	m_pNumberArea->setGeometry(QRect(cr.left(), cr.top(), NumberAreaWidth(), cr.height()));
}

void CLineNumberDecoration::UpdateNumberArea(QRect rect, int dy)
{
	if (dy)
		m_pNumberArea->scroll(0, dy);
	else
		m_pNumberArea->update(0, rect.y(), m_pNumberArea->width(), rect.height());

	if (rect.contains(viewport()->rect()))
		UpdateNumberAreaWidth();
}

int CLineNumberDecoration::NumberAreaWidth()
{
	int digits = DigitCount(qMax(1, blockCount()));

	if (digits < 3)
		digits = 3;

	int space = 10 + fontMetrics().width(QLatin1Char('0')) * digits;

	return space;
}

void CLineNumberDecoration::paintEvent(QPaintEvent* pEvent)
{
	QRect dr = pEvent->rect();
	IPlainTextEditDecorator::paintEvent(pEvent);
}

void CLineNumberDecoration::DrawNumberArea(QPaintEvent* pEvent)
{
	QPainter p(m_pNumberArea);

	QRect r = pEvent->rect();
	p.fillRect(pEvent->rect(), CSettings::GetGlobalDefaultSettings()->editor.numberarea.background);

	p.setPen(CSettings::GetGlobalDefaultSettings()->editor.numberarea.foreground);
	QTextBlock blk = firstVisibleBlock();
	int bn = blk.blockNumber();
	int t = (int)blockBoundingGeometry(blk).translated(contentOffset()).top();
	int b = t + (int)blockBoundingRect(blk).height();
	while (blk.isValid() && t <= pEvent->rect().bottom())
	{
		if (blk.isVisible() && b >= pEvent->rect().top())
			p.drawText(QRect(0, t, m_pNumberArea->width() - 5, fontMetrics().height()), Qt::AlignRight, QString::number(bn + 1));

		blk = blk.next();
		t = b;
		b = t + (int)blockBoundingRect(blk).height();
		++bn;
	}
}

void CLineNumberDecoration::resizeEvent(QResizeEvent* pEvent)
{
	QPlainTextEdit::resizeEvent(pEvent);
	UpdateNumberAreaWidth();
}