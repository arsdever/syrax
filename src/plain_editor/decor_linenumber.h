#pragma once

#include "plaineditdecor.h"

class QPaintEvent;
class QResizeEvent;
class CNumberArea;

class PLAIN_EDITOR_EXPORT CLineNumberDecoration : public IPlainTextEditDecorator
{

	Q_OBJECT

public:
	CLineNumberDecoration(QAbstractScrollArea* pCore);
	int NumberAreaWidth();
	void DrawNumberArea(QPaintEvent* pEvent);

	//operator CNumberArea*() { return m_pNumberArea; }

protected:
	void paintEvent(QPaintEvent* pEvent) override;
	void resizeEvent(QResizeEvent* pEvent) override;

public slots:
	void UpdateNumberArea(QRect rect, int dy);
	void UpdateNumberAreaWidth();

private:
	//CNumberArea* m_pNumberArea;
	int m_nLocalWidth;
};