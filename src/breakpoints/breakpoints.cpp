#include "breakpoints.h"
#include <core.h>
#include <QDebug>
#include <interfaces.h>
#include "../console/console_mgr.h"

CBreakpointManager::CBreakpointManager(QObject* pParent)
	: CPluginManager(pParent)
{
	QObject* pthis = qobject_cast<QObject*>(this);
	connect(this, SIGNAL(SigChanged(ui32)), this, SLOT(OnChanged(ui32)));
}

void CBreakpointManager::ToggleBreakpoint(ui32 nLine)
{
	i32 idx = IndexOf(nLine);
	if (idx == -1)
		m_aBreakpoints.push_back(nLine);
	else
		m_aBreakpoints.removeAt(idx);

	CConsoleManager* pConsoleManager = GetCore()->GetManager<CConsoleManager>();
	pConsoleManager->PrintInfoMessage(tr("Breakpoint toggled on line %0.").arg(nLine));

	emit SigChanged(nLine);
}

void CBreakpointManager::SetBreakpoint(ui32 nLine)
{
	if (IsSet(nLine))
		return;

	m_aBreakpoints.push_back(nLine);
	emit SigChanged(nLine);
}

void CBreakpointManager::RemoveBreakpoint(ui32 nLine)
{
	i32 idx = IndexOf(nLine);
	if (idx == -1)
		return;

	m_aBreakpoints.removeAt(idx);
	emit SigChanged(nLine);
}

void CBreakpointManager::ToggleBreakpoints(Range<ui32> rLines)
{
	//BlockNotifications();
	for (ui32 i = rLines.from; i < rLines.to; ++i)
	{
		ToggleBreakpoint(i);
		emit SigChanged(i);
	}
	//UnblockNotifications();
}

void CBreakpointManager::SetBreakpoints(Range<ui32> rLines)
{
	//BlockNotifications();
	for (ui32 i = rLines.from; i < rLines.to; ++i)
	{
		SetBreakpoint(i);
		emit SigChanged(i);
	}
	//UnblockNotifications();
	Notify();
}

void CBreakpointManager::RemoveBreakpoints(Range<ui32> rLines)
{
	//BlockNotifications();
	for (ui32 i = rLines.from; i < rLines.to; ++i)
	{
		RemoveBreakpoint(i);
		emit SigChanged(i);
	}
	//UnblockNotifications();
	Notify();
}

bool CBreakpointManager::IsSet(ui32 nLine)
{
	return IndexOf(nLine) > -1;
}

i32 CBreakpointManager::IndexOf(ui32 nLine)
{
	return m_aBreakpoints.indexOf(nLine);
}

void CBreakpointManager::OnChanged(ui32 nLine)
{
	for (auto it : m_lstPlugins)
	{
		IBreakpoint* pInterface = dynamic_cast<IBreakpoint*>(it);

		if (pInterface == nullptr)
			continue;

		pInterface->ToggleBreakpoint(nLine);
	}
}

void CBreakpointManager::Notify()
{
}