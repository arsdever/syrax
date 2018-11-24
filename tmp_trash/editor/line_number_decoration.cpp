#include "line_number_decoration.h"
#include "decorated_editor.h"


CLineNumberDecoration::CLineNumberDecoration(CDecoratedEditor* pParent)
	: IEditorDecoration(pParent)
{
	int space = 5 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * DigitCount();
	setFixedWidth(space);
}

CLineNumberDecoration::~CLineNumberDecoration()
{}

QSize CLineNumberDecoration::sizeHint() const
{
	int space = 5 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * DigitCount();
	
	return QSize(space, 0);
}

int CLineNumberDecoration::DigitCount() const
{
	int digits = 1;
	int max = qMax(1, GetEditor()->blockCount());
	while (max >= 10) {
		max /= 10;
		++digits;
	}

	return digits;
}

void CLineNumberDecoration::paintEvent(QPaintEvent* pEvent)
{
	GetEditor()->DrawLineNumberArea(pEvent, this);
}