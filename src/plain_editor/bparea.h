#pragma once

#include <QWidget>
#include <macros.h>
#include <typedef.h>
#include <interfaces.h>
#include <bpmanager.h>
#include "plain_editor_global.h"
#include "decor_breakpoint.h"

class QPaintEvent;
class QMouseEvent;
class QPlainTextEdit;

class PLAIN_EDITOR_EXPORT CBreakpointArea :	public QWidget
{

	REGISTER_INTERFACE(CBreakpointArea, Breakpoint)
		void ToggleBreakpoint(int);
	REGISTER_INTERFACE_END(Breakpoint)

public:
	CBreakpointArea(CBreakpointDecoration* pParent);

	QSize sizeHint();
	inline QList<ui32> GetBreakpointList() const { return m_aBreakpoints; }
	void RegisterManager(CBreakpointManager* pManager);

private:
	void paintEvent(QPaintEvent* pEvent);
	void mousePressEvent(QMouseEvent* pEvent);

private:
	QPlainTextEdit* m_pEditor;
	QList<unsigned int> m_aBreakpoints;
	CBreakpointManager* m_pBPManager;
};

