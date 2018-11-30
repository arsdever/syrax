#pragma once

#include "console_global.h"

#include <interfaces.h>
#include <macros.h>

#include <QWidget>
#include <QTextEdit>

class QKeyEvent;
class QMenuBar;

extern "C" CONSOLE_EXPORT void LoadPlugin(QMenuBar*);

class CONSOLE_EXPORT CConsole : public QTextEdit
{

	Q_OBJECT

	REGISTER_INTERFACE(CConsole, FileManipulator)
		virtual void New() override;

		virtual void Open(QStringList const&) override;

		virtual void Save(QString const& = "") override;
		virtual void SaveAs() override;
		virtual void SaveAll() override;

		virtual void Close(qint32 = -1) override;

		virtual bool AskForClose(qint32, EClosingType) override;
	REGISTER_INTERFACE_END(FileManipulator)

	REGISTER_INTERFACE(CConsole, Application)
		virtual void Close() override;
		virtual void AddDockWidget(QWidget*, QString const&, Qt::DockWidgetArea) override;
		virtual void RemoveDockWidget(QWidget*) override;
	REGISTER_INTERFACE_END(Application)

	REGISTER_INTERFACE(CConsole, Logger)
		virtual void Info(QString const&) override;
		virtual void Warning(QString const&) override;
		virtual void Error(QString const&) override;
	REGISTER_INTERFACE_END(Logger)

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
