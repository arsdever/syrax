#include "decorated_editor.h"
#include "public_plain_text_edit.h"
#include <QLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QTextBlock>

CDecoratedEditor::CDecoratedEditor(QWidget* pParent)
	: QPlainTextEdit(pParent)
{
	SetupUI();
}

CDecoratedEditor::~CDecoratedEditor()
{
}

void CDecoratedEditor::DrawLineNumberArea(QPaintEvent* pEvent, QWidget* pWidget)
{
	QPainter painter(pWidget);
	painter.fillRect(pEvent->rect(), Qt::lightGray);

	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int)blockBoundingRect(block).height();

	while (block.isValid() && top <= pEvent->rect().bottom()) {
		if (block.isVisible() && bottom >= pEvent->rect().top()) {
			QString number = QString::number(blockNumber + 1);
			painter.setPen(Qt::black);
			painter.drawText(0, top, width(), fontMetrics().height(),
				Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + (int)blockBoundingRect(block).height();
		++blockNumber;
	}
}

void CDecoratedEditor::SetupUI()
{
	QLayout* pLayout = layout();

	if(pLayout != nullptr)
	while(pLayout->count())
		pLayout->takeAt(0);

	delete pLayout;
	
	setLayout(new QHBoxLayout());
	layout()->setMargin(0);
	layout()->setSpacing(0);
	for (QWidget* widget : m_arrArrangement)
	{
		layout()->addWidget(widget);
		widget->show();
	}
}

void CDecoratedEditor::AddDecoration(QWidget* pWidget, Qt::Alignment eAlignment)
{
	if (eAlignment == Qt::AlignLeft || eAlignment == Qt::AlignTop)
		m_arrArrangement.push_front(pWidget);
	else if(eAlignment == Qt::AlignRight || eAlignment == Qt::AlignBottom)
		m_arrArrangement.push_back(pWidget);

	SetupUI();
}