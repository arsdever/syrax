#pragma once

#include <QWidget>
#include "console_global.h"
#include <interfaces.h>
#include <macros.h>
#include <QTextEdit>

class QKeyEvent;
class QMenuBar;

extern "C" CONSOLE_EXPORT void LoadPlugin(QMenuBar*);

class CONSOLE_EXPORT CConsole : public QTextEdit
{

	Q_OBJECT

	REGISTER_INTERFACE(CConsole, FileManipulator)
		virtual void Open(QStringList const&) override;
		virtual void CloseAll(int) override;
		virtual void SaveAll() override;
		virtual void Close(int) override;
		virtual void Save(QString const&) override;
		virtual void New() override;
	REGISTER_INTERFACE_END(FileManipulator)

	REGISTER_INTERFACE(CConsole, Breakpoint)
		virtual void ToggleBreakpoint(unsigned int) override;
	REGISTER_INTERFACE_END(Breakpoint)

	REGISTER_INTERFACE(CConsole, Application)
		virtual void Close() override;
		virtual void AddDockWidget(QWidget*, QString const&, Qt::DockWidgetArea) override;
		virtual void RemoveDockWidget(QWidget*) override;
	REGISTER_INTERFACE_END(Application)

	CConsole(QWidget* pParent = nullptr);
public:
	~CConsole() {}

	friend CONSOLE_EXPORT  void LoadPlugin(QMenuBar*);

protected:
	void keyPressEvent(QKeyEvent* pEvent) override;
	//void keyReleaseEvent(QKeyEvent* pEvent) override;
	void ParseInputString();

public slots:
	void ViewActionTriggered(bool bChecked);
	void OnClear();
	void OnExecute();

private:
	QString m_strInputString;
};
