#pragma once

#include "console_global.h"

#include <core.h>

#include <QWidget>
#include <QTextEdit>

class QKeyEvent;
class QEvent;
class QMenuBar;

extern "C" CONSOLE_EXPORT void LoadPlugin(QMenuBar*);
extern "C" CONSOLE_EXPORT void UnloadPlugin(QMenuBar*);

class CONSOLE_EXPORT CConsole : public QTextEdit
{

	Q_OBJECT
		
	REGISTER_INTERFACE(CConsole, Logger)
		virtual void Info(QString const&) override;
		virtual void Warning(QString const&) override;
		virtual void Error(QString const&) override;
	REGISTER_INTERFACE_END(Logger)

	CConsole(QWidget* pParent = nullptr);
public:
	~CConsole() {}

	static CConsole* instance();
	friend CONSOLE_EXPORT void LoadPlugin(QMenuBar*);
	friend CONSOLE_EXPORT void UnloadPlugin(QMenuBar*);

protected:
	void keyPressEvent(QKeyEvent* pEvent) override;
	void ParseInputString();

public slots:
	void ViewActionTriggered(bool bChecked);
	void OnClear();
	void OnExecute();

private:
	QString m_strInputString;
	static CConsole* s_pInstance;
};
