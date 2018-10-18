#pragma once

#include "breakpoints_global.h"
#include <pluginmgr.h>
#include <manipulators.h>
#include <typedef.h>
#include <notifier.h>

#include <QObject>
#include <QList>

class BREAKPOINTS_EXPORT CBreakpointManager : public CPluginManager//, public INotifier
{

	Q_OBJECT

public:
	CBreakpointManager(QObject* pParent = nullptr);

	void ToggleBreakpoint(ui32 nLine);
	void SetBreakpoint(ui32 nLine);
	void RemoveBreakpoint(ui32 nLine);
	void ToggleBreakpoints(Range<ui32> rLines);
	void SetBreakpoints(Range<ui32> rLines);
	void RemoveBreakpoints(Range<ui32> rLines);
	bool IsSet(ui32 nLine);

	void Notify();

protected:
	i32 IndexOf(ui32 nLine);

protected slots:
	void OnChanged(ui32);

signals:
	void SigChanged(ui32);

private:
	QList<ui32> m_aBreakpoints;
};