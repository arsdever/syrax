#include "plaintexteditdecor.h"

IPlainTextEditDecorator::IPlainTextEditDecorator(QPlainTextEdit* pCore)
	: IDecoration(pCore)
{
}

IPlainTextEditDecorator::~IPlainTextEditDecorator()
{
}
