#pragma once

#include <QPlainTextEdit>
#include <typedef.h>
#include "plain_editor_global.h"

class CBreakpointArea;

class PLAIN_EDITOR_EXPORT CPlainEditor : public QPlainTextEdit
{

	Q_OBJECT

public:
	CPlainEditor(QWidget* pParent = 0);

	void SetupUI();

	// Numbering area


	// Breakpoint area

	int BreakpointAreaWidth();
	void DrawBreakpointArea(QPaintEvent* pEvent);
	//void SetBreakpointEvent(QMouseEvent* pEvent);
	int GetLineIndexByCoordinate(int nYPos);
	//void ToggleBreakpoint(int nLineNumber);
	CBreakpointArea* BreakpointArea() { return m_pBreakpointArea; }
	ui32 CurrentLineNumber();

protected:
	void resizeEvent(QResizeEvent* pEvent);

private slots:
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect &, int);/*
	void OnBPSet(unsigned int nLineNumber);
	void OnBPUnset(unsigned int nLineNumber);

signals:
*/
private:
	CBreakpointArea *m_pBreakpointArea;
	int m_nExecutionLine;
};