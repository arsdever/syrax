#pragma once

#include "plain_editor_global.h"

#include <decoration.h>
#include "publicplaintext.h"

class IPlainTextEditDecorator : public IDecoration<QAbstractScrollArea>
{
public:
	IPlainTextEditDecorator(QAbstractScrollArea* pCore);
	virtual ~IPlainTextEditDecorator();

protected:
	virtual void paintEvent(QPaintEvent* pEvent) override
	{ 
		CoreWidget()->update(pEvent->rect());
		//IDecoration::paintEvent(pEvent);
	}
};