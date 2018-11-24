#include "console.h"

#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QApplication>
#include <core.h>
#include <QKeyEvent>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

#ifdef QT_DEBUG
#include <QDebug>
#endif

CConsole::CConsole(QWidget* pParent)
	: QTextEdit(pParent)
{
	setReadOnly(true);
}

void CConsole::keyPressEvent(QKeyEvent* pEvent)
{
	switch (pEvent->key())
	{
	case Qt::Key_Enter:
		ParseInputString();
		break;
	case Qt::Key_Backspace:
		if (m_strInputString == "")
			break;

		QTextEdit::keyPressEvent(pEvent);

		if (m_strInputString[m_strInputString.length() - 1] == '\n')
			m_strInputString.chop(2);
		else
			m_strInputString.chop(1);

		break;
	default:
		m_strInputString += pEvent->text();
		QTextEdit::keyPressEvent(pEvent);
	}
}

void CConsole::ParseInputString()
{
	// magic happens
}
//
//void CConsole::XConsole::PrintMessage(QString const& str)
//{
//	m_pThis->append(tr("%0 : %1").arg("[INFO] ", 10).arg(str));
//}
//
//void CConsole::XConsole::PrintError(QString const& str)
//{
//	m_pThis->append(tr("%0 : %1").arg("[ERROR]", 10).arg(str));
//}
//
//void CConsole::XConsole::PrintWarning(QString const& str)
//{
//	m_pThis->append(tr("%0 : %1").arg("[WARN] ", 10).arg(str));
//}
//
//void CConsole::XConsole::GetInput(QString const& str)
//{
//	m_pThis->append(str);
//}


void CConsole::XFileManipulator::Open(QStringList const& strPaths) 
{
	if (strPaths.empty())
		m_pThis->append(QString("No file choosen for opening"));
	else if (strPaths.size() > 1)
		m_pThis->append(QString("Opening multiple files [\"%1\"]").arg(strPaths.join("\",\"")));
	else
		m_pThis->append(QString("Opening file \"%1\"").arg(strPaths.front()));
}
void CConsole::XFileManipulator::CloseAll(int nIndex) 
{
	if (nIndex == 0)
		m_pThis->append(QString("Closing all tabs"));
	else
		m_pThis->append(QString("Closing all tabs after %1").arg(nIndex));
}
void CConsole::XFileManipulator::SaveAll() 
{
	m_pThis->append(QString("Saving all tabs"));
}
void CConsole::XFileManipulator::Close(int nIndex)
{
	m_pThis->append(nIndex < 0 ? QString("Closing current tab") : QString("Closing tab by index %1").arg(nIndex));
}
void CConsole::XFileManipulator::Save(QString const& strPath) 
{
	if (strPath == "")
	{
		m_pThis->append(QString("Saving current tab"));
	}
	else
	{
		m_pThis->append(QString("Saving current tab to path %1").arg(strPath));
	}
}
void CConsole::XFileManipulator::New()
{
	m_pThis->append(QString("Opening new tab"));
}
void CConsole::XBreakpoint::ToggleBreakpoint(unsigned int nLine)
{
	m_pThis->append(QString("Toggling breakpoint at line %1").arg(nLine));
}

void CConsole::ViewActionTriggered(bool bChecked)
{
	QSet<IApplication*> plgList = GetCore()->QueryInterface<IApplication>();
	if (plgList.empty())
		return;

	IApplication* pApplication = *plgList.begin();

	if(bChecked)
		pApplication->AddDockWidget(this, "Console", Qt::BottomDockWidgetArea);
	else
		pApplication->RemoveDockWidget(this);
}

void CConsole::XApplication::Close()
{
	m_pThis->append(tr("Closing application."));
}

void CConsole::XApplication::AddDockWidget(QWidget*, QString const& title, Qt::DockWidgetArea)
{
	m_pThis->append(tr("New dock widget request %1.").arg(title));
}

void CConsole::XApplication::RemoveDockWidget(QWidget*)
{
	m_pThis->append(tr("Remove dock widget request."));
}

void CConsole::OnClear()
{
	clear();
}

void CConsole::OnExecute()
{

}

extern "C" CONSOLE_EXPORT void LoadPlugin(QMenuBar* pMenuBar)
{
	CConsole* pInstance = new CConsole();
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

	QAction* pAction = viewMenu->addAction("Console", pInstance, SLOT(ViewActionTriggered(bool)));

	pAction->setCheckable(true);
	pAction->setChecked(false);

	QMenu* pMenu = pMenuBar->addMenu("Console");
	pMenu->addAction("Clear console", pInstance, SLOT(OnClear()));
	pMenu->addAction("Execute command", pInstance, SLOT(OnExecute()));
}