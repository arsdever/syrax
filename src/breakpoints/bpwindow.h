#pragma once

#include "breakpoints_global.h"
#include <QListWidget>
#include <macros.h>
#include <typedef.h>
#include <interfaces.h>

class CBreakpointManager;

class BREAKPOINTS_EXPORT CBreakpointWindow : public QListWidget
{

	Q_OBJECT

	REGISTER_INTERFACE(CBreakpointWindow, Breakpoint)
		void ToggleBreakpoint(int nLine);
	REGISTER_INTERFACE_END(Breakpoint)

public:
	CBreakpointWindow(QWidget* pParent = nullptr);
	~CBreakpointWindow();

	void SetManager(CBreakpointManager* pManager);

private:
	CBreakpointManager* m_pManager;
};