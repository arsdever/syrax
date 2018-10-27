#include "decor_linenumber.h"
#include "numarea.h"

#include <settings.h>
#include <QTextBlock>
#include <manipulators.h>
#include <QResizeEvent>
#include <QPainter>

CLineNumberDecoration::CLineNumberDecoration(QAbstractScrollArea* pCore)
	: IPlainTextEditDecorator(pCore)
	//, m_pNumberArea(new CNumberArea(this))
	, m_nLocalWidth(0)
{
	UpdateNumberAreaWidth();
	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(UpdateNumberAreaWidth()));
	connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(UpdateNumberArea(QRect, int)));
}

void CLineNumberDecoration::UpdateNumberAreaWidth()
{
	CPublicPlainTextEdit* editor = dynamic_cast<CPublicPlainTextEdit*>(CoreWidget());
	if (editor == nullptr)
		return;

	int wdt = NumberAreaWidth();
	QMargins mgs = editor->viewportMargins();
	mgs += QMargins(wdt, 0, 0, 0);
	mgs -= QMargins(m_nLocalWidth, 0, 0, 0);
	m_nLocalWidth = wdt;
	editor->setViewportMargins(mgs);

	QRect cr = contentsRect();
	setGeometry(QRect(cr.left(), cr.top(), NumberAreaWidth(), cr.height()));
}

void CLineNumberDecoration::UpdateNumberArea(QRect rect, int dy)
{
	CPublicPlainTextEdit* editor = dynamic_cast<CPublicPlainTextEdit*>(CoreWidget());
	if (editor == nullptr)
		return;

	if (dy)
		scroll(0, dy);
	else
		update(0, rect.y(), width(), rect.height());

	if (rect.contains(editor->viewport()->rect()))
		UpdateNumberAreaWidth();
}

int CLineNumberDecoration::NumberAreaWidth()
{
	CPublicPlainTextEdit* editor = dynamic_cast<CPublicPlainTextEdit*>(CoreWidget());
	if (editor == nullptr)
		return -1;

	int digits = DigitCount(qMax(1, editor->blockCount()));

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
	CPublicPlainTextEdit* editor = dynamic_cast<CPublicPlainTextEdit*>(CoreWidget());
	if (editor == nullptr)
		return;

	QPainter p(this);

	QRect r = pEvent->rect();
	p.fillRect(pEvent->rect(), CSettings::GetGlobalDefaultSettings()->editor.numberarea.background);

	p.setPen(CSettings::GetGlobalDefaultSettings()->editor.numberarea.foreground);
	QTextBlock blk = editor->firstVisibleBlock();
	int bn = blk.blockNumber();
	int t = (int)editor->blockBoundingGeometry(blk).translated(editor->contentOffset()).top();
	int b = t + (int)editor->blockBoundingRect(blk).height();
	while (blk.isValid() && t <= pEvent->rect().bottom())
	{
		if (blk.isVisible() && b >= pEvent->rect().top())
			p.drawText(QRect(0, t, width() - 5, fontMetrics().height()), Qt::AlignRight, QString::number(bn + 1));

		blk = blk.next();
		t = b;
		b = t + (int)editor->blockBoundingRect(blk).height();
		++bn;
	}
}

void CLineNumberDecoration::resizeEvent(QResizeEvent* pEvent)
{
	QWidget::resizeEvent(pEvent);
	UpdateNumberAreaWidth();
}