#include "bparea.h"
#include <QPlainTextEdit>
#include <decorations.h>
#include "../plugin/pluginmgr_base.h"
#include <core.h>

CBreakpointArea::CBreakpointArea(CBreakpointDecoration* pParent)
	: QWidget(pParent)
{
	m_pBPManager = GetCore()->GetManager<CBreakpointManager>();
	m_pBPManager->RegisterInterface(&m_xBreakpoint);
}

QSize CBreakpointArea::sizeHint() {
//	return QSize(m_pEditor->BreakpointAreaWidth(), 0);
	return QSize();
}

void CBreakpointArea::paintEvent(QPaintEvent* pEvent)
{
	dynamic_cast<CBreakpointDecoration*>(parent())->DrawBreakpointArea(pEvent);
}

void CBreakpointArea::mousePressEvent(QMouseEvent* pEvent)
{
	int index = dynamic_cast<CBreakpointDecoration*>(parent())->GetLineIndexByCoordinate(pEvent->y());

	if (index < 0)
		return;

	m_pBPManager->ToggleBreakpoint(index + 1);
}

void CBreakpointArea::XBreakpoint::ToggleBreakpoint(int nLine)
{
	int idx = m_pThis->m_aBreakpoints.indexOf(nLine);

	if (idx > -1)
		m_pThis->m_aBreakpoints.removeAt(idx);
	else
		m_pThis->m_aBreakpoints.push_back(nLine);

	m_pThis->update();
}