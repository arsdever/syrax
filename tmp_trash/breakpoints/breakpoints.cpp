#include "breakpoints.h"
#include <core.h>
#include <QDebug>
#include <bpwindow.h>
#include <QMenuBar>
#include <editormgr.h>

CBreakpoints::CBreakpoints(QObject* pParent)
	: QObject(pParent)
{
}

void CBreakpoints::XBreakpoint::ToggleBreakpoint(ui32 nLine)
{
	m_pThis->ToggleBreakpoint(nLine);
}

void CBreakpoints::ToggleBreakpoint(ui32 nLine)
{
	i32 idx = IndexOf(nLine);
	if (idx == -1)
		m_aBreakpoints.push_back(nLine);
	else
		m_aBreakpoints.removeAt(idx);
}

void CBreakpoints::SetBreakpoint(ui32 nLine)
{
	if (IsSet(nLine))
		return;

	m_aBreakpoints.push_back(nLine);
}

void CBreakpoints::RemoveBreakpoint(ui32 nLine)
{
	i32 idx = IndexOf(nLine);
	if (idx == -1)
		return;

	m_aBreakpoints.removeAt(idx);
}

void CBreakpoints::ToggleBreakpoints(Range<ui32> rLines)
{
	//BlockNotifications();
	for (ui32 i = rLines.from; i < rLines.to; ++i)
	{
		ToggleBreakpoint(i);
	}
	//UnblockNotifications();
}

void CBreakpoints::SetBreakpoints(Range<ui32> rLines)
{
	for (ui32 i = rLines.from; i < rLines.to; ++i)
	{
		SetBreakpoint(i);
	}
}

void CBreakpoints::RemoveBreakpoints(Range<ui32> rLines)
{
	//BlockNotifications();
	for (ui32 i = rLines.from; i < rLines.to; ++i)
	{
		RemoveBreakpoint(i);
	}
}

bool CBreakpoints::IsSet(ui32 nLine)
{
	return IndexOf(nLine) > -1;
}

i32 CBreakpoints::IndexOf(ui32 nLine)
{
	return m_aBreakpoints.indexOf(nLine);
}

QList<ui32> CBreakpoints::GetBreakpointList() const
{
	return m_aBreakpoints;
}

QWidget* CBreakpoints::XWidgetProvider::GetWidget(QString const& strName)
{
	if(strName == "Breakpoint")
		return new CBreakpointWindow();

	return nullptr;
}

void CBreakpoints::ViewActionTriggered(bool bChecked)
{
	QSet<IApplication*> plgList = GetCore()->QueryInterface<IApplication>();
	IApplication* pApplication = *plgList.begin();

	if (pApplication == nullptr)
		return;

	if (bChecked)
		pApplication->AddDockWidget(CBreakpointWindow::GlobalInstance(), "Breakpoint", Qt::RightDockWidgetArea);
	else
		pApplication->RemoveDockWidget(CBreakpointWindow::GlobalInstance());
}

void CBreakpoints::OnToggleBreakpoint(ui32 nLine)
{
	QSet<IBreakpoint*> plgList = GetCore()->QueryInterface<IBreakpoint>();

	for (IBreakpoint* plg : plgList)
		plg->ToggleBreakpoint(curentLine());
}

void CBreakpoints::OnClearBreakpoints(ui32 nLine)
{
}

extern "C" BREAKPOINTS_EXPORT void LoadPlugin(QMenuBar* pMenuBar)
{
	CBreakpoints* pBps = new CBreakpoints(GetCore());
	if (pMenuBar == nullptr)
		return;

	QList<QMenu*> menuList = pMenuBar->findChildren<QMenu*>();
	QMenu* viewMenu = nullptr;
	for (QMenu* pMenu : menuList)
	{
		if (pMenu->title() != "View")
			continue;

		viewMenu = pMenu;
		break;
	}

	if (viewMenu == nullptr)
		viewMenu = pMenuBar->addMenu("View");

	QAction* pAction = viewMenu->addAction("Breakpoint", pBps, SLOT(ViewActionTriggered(bool)));

	pAction->setCheckable(true);
	pAction->setChecked(false);

	QMenu* bpmenu = pMenuBar->addMenu("Breakpoint");
	bpmenu->addAction("Toggle breakpoint", pBps, SLOT(OnToggleBreakpoint()), QKeySequence("F9"));
	bpmenu->addAction("Clear breakpoints", pBps, SLOT(OnClearBreakpoints()));
}