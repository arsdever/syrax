#include "bpwindow.h"
#include <bpmanager.h>
#include <core.h>

CBreakpointWindow* CBreakpointWindow::s_pGlobalInstance = nullptr;

CBreakpointWindow::CBreakpointWindow(QWidget* pParent)
	: QListWidget(pParent)
{
	GetCore()->RegisterInterface(&m_xBreakpoint);
}

CBreakpointWindow::~CBreakpointWindow()
{
}

void CBreakpointWindow::XBreakpoint::ToggleBreakpoint(unsigned int nLine)
{/*
	QString itemText = tr("Breakpoint at line %1").arg(nLine);
	QList<QListWidgetItem*> list = m_pThis->findItems(itemText, Qt::MatchExactly);

	if (list.empty())
		m_pThis->addItem(itemText);
	else
		delete m_pThis->takeItem(m_pThis->row(list.at(0)));*/
}

CBreakpointWindow* CBreakpointWindow::GlobalInstance()
{
	return s_pGlobalInstance == nullptr ? s_pGlobalInstance = new CBreakpointWindow() : s_pGlobalInstance;
}
