#pragma once

#include <pluginmgr.h>
#include "console_global.h"
#include <macros.h>

EXPORT_CLASS(CONSOLE)

class CONSOLE_EXPORT CConsoleManager : public CPluginManager
{

	Q_OBJECT

	CConsoleManager(QObject* pParent = nullptr);
	DECLARE_MANAGER(CONSOLE)

public:
	~CConsoleManager();

	void PrintInfoMessage(QString const& msg);
	void PrintWarningMessage(QString const& msg);
	void PrintErrorMessage(QString const& msg);

	QString GetManagerName() const override
	{
		return "ConsoleManager";
	}
};
