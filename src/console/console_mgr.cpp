#include "console_mgr.h"
#include <core.h>
#include <interfaces.h>

CConsoleManager::CConsoleManager(QObject* pParent)
	: CPluginManager(pParent)
{
}

CConsoleManager::~CConsoleManager()
{
}

void CConsoleManager::PrintInfoMessage(QString const& msg)
{
	for (auto it : m_lstPlugins)
	{
		IConsole* pConsole = dynamic_cast<IConsole*>((IUnknown*)it);

		if (pConsole == nullptr)
			continue;

		pConsole->PrintMessage(msg);
	}
}

void CConsoleManager::PrintWarningMessage(QString const& msg)
{
	for (auto it : m_lstPlugins)
	{
		IConsole* pConsole = dynamic_cast<IConsole*>(it);

		if (pConsole == nullptr)
			continue;

		pConsole->PrintWarning(msg);
	}
}

void CConsoleManager::PrintErrorMessage(QString const& msg)
{
	for (auto it : m_lstPlugins)
	{
		IConsole* pConsole = dynamic_cast<IConsole*>(it);

		if (pConsole == nullptr)
			continue;

		pConsole->PrintError(msg);
	}
}

DEFINE_MANAGER(CONSOLE, CConsoleManager)