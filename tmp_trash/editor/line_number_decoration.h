#pragma once

#include "editor_decoration.h"

class CLineNumberDecoration : public IEditorDecoration
{
public:
	CLineNumberDecoration(CDecoratedEditor* pParent);
	~CLineNumberDecoration() override;

	int DigitCount() const;

protected:
	QSize sizeHint() const override;
	void paintEvent(QPaintEvent *pEvent) override;
};