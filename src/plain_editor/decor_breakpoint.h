#pragma once

#include <typedef.h>
#include "plaineditdecor.h"

class QPaintEvent;
class QResizeEvent;
class CBreakpointArea;
class CBreakpointManager;

class PLAIN_EDITOR_EXPORT CBreakpointDecoration : public IPlainTextEditDecorator
{

	Q_OBJECT

public:
	CBreakpointDecoration(QPlainTextEdit* pCore);
	int BreakpointAreaWidth();
	void DrawBreakpointArea(QPaintEvent* pEvent);

	void RegisterManager(CBreakpointManager* pManager);
	ui32 GetLineIndexByCoordinate(ui32 nYPos);

protected:
	void paintEvent(QPaintEvent* pEvent) override;
	void resizeEvent(QResizeEvent* pEvent) override;

public slots:
	void UpdateBreakpointArea(QRect rect, int dy);
	void UpdateBreakpointAreaWidth();

private:
	CBreakpointArea* m_pBreakpointArea;
	int m_nLocalWidth;
};