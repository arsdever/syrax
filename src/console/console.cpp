#include "console.h"

#include <QKeyEvent>
#include <QMenuBar>

CConsole* CConsole::s_pInstance = nullptr;

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

void CConsole::XLogger::Info(QString const& strLog)
{
	m_pThis->append("[INFO]    " + strLog);
}

void CConsole::XLogger::Warning(QString const& strLog)
{
	m_pThis->append("[WARNING] " + strLog);
}

void CConsole::XLogger::Error(QString const& strLog)
{
	m_pThis->append("[ERROR]   " + strLog);
}

void CConsole::ViewActionTriggered(bool bChecked)
{
	if (bChecked)
	{
		CallFunction<IApplication>(IApplication::AddDockWidgetFunctor(this, "Console", Qt::BottomDockWidgetArea));
		CallFunction<ILogger>(ILogger::InfoFunctor("Console widget showing."));
	}
	else
	{
		CallFunction<IApplication>(IApplication::RemoveDockWidgetFunctor(this));
		CallFunction<ILogger>(ILogger::InfoFunctor("Console widget closing."));
	}
}

void CConsole::OnClear()
{
	clear();
}

void CConsole::OnExecute()
{

}

CConsole* CConsole::instance()
{
	return s_pInstance == nullptr ? s_pInstance = new CConsole() : s_pInstance;
}

extern "C" CONSOLE_EXPORT void LoadPlugin(QMenuBar* pMenuBar)
{
	CConsole* pInstance = CConsole::instance();

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

extern "C" CONSOLE_EXPORT void UnloadPlugin(QMenuBar* pMenuBar)
{
	CallFunction<IApplication>(IApplication::RemoveDockWidgetFunctor(CConsole::instance()));
	delete CConsole::instance();
	CConsole::s_pInstance = nullptr;

	if (pMenuBar == nullptr)
		return;

	QList<QMenu*> menuList = pMenuBar->findChildren<QMenu*>();
	QMenu* viewMenu = nullptr;
	for (QMenu* pMenu : menuList)
	{
		if (pMenu->title() == "Console")
		{
			delete pMenu;
			continue;
		}

		if (pMenu->title() != "View")
			continue;

		viewMenu = pMenu;
	}

	if (viewMenu == nullptr)
		viewMenu = pMenuBar->addMenu("View");

	QList<QAction*> actionList = viewMenu->findChildren<QAction*>();
	for (QAction* pAction : actionList)
	{
		if (pAction->text() == "Console")
		{
			delete pAction;
			break;
		}
	}
	if (viewMenu->findChildren<QAction*>().empty())
		delete viewMenu;
}