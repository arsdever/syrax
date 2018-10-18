#pragma once

#include <typedef.h>
#include <plaineditdecor.h>
#include <QWidget>

class QPaintEvent;
class CLineNumberDecoration;

class CNumberArea :	public QWidget
{
public:
	CNumberArea(CLineNumberDecoration* pParent);

	void paintEvent(QPaintEvent* pEvent) override;
};

