#pragma once

#include "breakpoints_global.h"
#include <interfaces.h>
#include <manipulators.h>
#include <typedef.h>
#include <macros.h>

#include <QObject>
#include <QList>

class BREAKPOINTS_EXPORT CBreakpoints : public QObject
{

	Q_OBJECT
		
	REGISTER_INTERFACE(CBreakpoints, Breakpoint)
		virtual void ToggleBreakpoint(unsigned int) override;
	REGISTER_INTERFACE_END(Breakpoint)
		
	REGISTER_INTERFACE(CBreakpoints, WidgetProvider)
		virtual QWidget* GetWidget(QString const&) override;
	REGISTER_INTERFACE_END(WidgetProvider)

public:
	CBreakpoints(QObject* pParent = nullptr);
	void ToggleBreakpoint(ui32 nLine);
	void SetBreakpoint(ui32 nLine);
	void RemoveBreakpoint(ui32 nLine);
	void ToggleBreakpoints(Range<ui32> rLines);
	void SetBreakpoints(Range<ui32> rLines);
	void RemoveBreakpoints(Range<ui32> rLines);
	QList<ui32> GetBreakpointList() const;
	bool IsSet(ui32 nLine);

protected:
	i32 IndexOf(ui32 nLine);

public slots:
	void ViewActionTriggered(bool);
	void OnToggleBreakpoint(ui32 nLine);
	void OnClearBreakpoints(ui32 nLine);

private:
	QList<ui32> m_aBreakpoints;
};