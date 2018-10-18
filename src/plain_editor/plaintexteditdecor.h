#pragma once

#include "plain_editor_global.h"

#include <decoration.h>
#include <QPlainTextEdit>

class IPlainTextEditDecorator : public IDecoration<QPlainTextEdit>
{
public:
	IPlainTextEditDecorator(QPlainTextEdit* pCore);
	virtual ~IPlainTextEditDecorator();

protected:
	virtual void paintEvent(QPaintEvent* pEvent) override
	{ 
		// CoreWidget()->update(pEvent->rect());
		IDecoration::paintEvent(pEvent);
	}
};