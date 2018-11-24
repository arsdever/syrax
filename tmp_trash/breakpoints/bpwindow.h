#pragma once

#include "breakpoints_global.h"
#include <QListWidget>
#include <macros.h>
#include <typedef.h>
#include <interfaces.h>


class BREAKPOINTS_EXPORT CBreakpointWindow : public QListWidget
{

	Q_OBJECT

	REGISTER_INTERFACE(CBreakpointWindow, Breakpoint)
		void ToggleBreakpoint(unsigned int nLine);
	REGISTER_INTERFACE_END(Breakpoint)

public:
	CBreakpointWindow(QWidget* pParent = nullptr);
	~CBreakpointWindow();

	//void SetManager(CBreakpointManager* pManager);
	static CBreakpointWindow* GlobalInstance();

private:
	static CBreakpointWindow* s_pGlobalInstance;
	//CBreakpointManager* m_pManager;
};