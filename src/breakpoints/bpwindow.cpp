#include "bpwindow.h"
#include <bpmanager.h>
#include <core.h>

CBreakpointWindow::CBreakpointWindow(QWidget* pParent)
	: QListWidget(pParent)
{
	m_pManager = GetCore()->GetManager<CBreakpointManager>();
	m_pManager->RegisterInterface(&m_xBreakpoint);
}

CBreakpointWindow::~CBreakpointWindow()
{
}

void CBreakpointWindow::XBreakpoint::ToggleBreakpoint(int nLine)
{
	QString itemText = tr("Breakpoint at line %1").arg(nLine);
	QList<QListWidgetItem*> list = m_pThis->findItems(itemText, Qt::MatchExactly);

	if (list.empty())
		m_pThis->addItem(itemText);
	else
		delete m_pThis->takeItem(m_pThis->row(list.at(0)));
}