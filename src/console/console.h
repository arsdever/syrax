#pragma once

#include <QWidget>
#include "console_global.h"
#include <interfaces.h>
#include <macros.h>
#include <QTextEdit>

class CConsoleManager;
class QKeyEvent;

class CONSOLE_EXPORT CConsole : public QTextEdit
{

	Q_OBJECT

	REGISTER_INTERFACE(CConsole, Console)
		void PrintMessage(QString const&);
		void PrintError(QString const&);
		void PrintWarning(QString const&);
		void GetInput(QString const&);
	REGISTER_INTERFACE_END(Console)

public:
	CConsole(QWidget* pParent = nullptr);
	~CConsole() {}

protected:
	void keyPressEvent(QKeyEvent* pEvent) override;
	//void keyReleaseEvent(QKeyEvent* pEvent) override;
	void ParseInputString();

private:
	CConsoleManager* m_pManager;
	QString m_strInputString;
};
