#include "number_area_decor.h"

CNumberAreaDecoration::CNumberAreaDecoration(QWidget* pParent)
	: QWidget(pParent)
{
	setFixedWidth(50);
	setMinimumWidth(50);
}

CNumberAreaDecoration::~CNumberAreaDecoration()
{
}
