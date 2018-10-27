#pragma once

#include "application_global.h"
#include <QMainWindow>
#include <bpmanager.h>
#include <interfaces.h>
#include <macros.h>
#include "application_mgr.h"

class QPlainTextEdit;
class CBreakpointWindow;
class CConsole;

class APPLICATION_EXPORT CApplication : public QMainWindow
{
	Q_OBJECT

	REGISTER_INTERFACE(CApplication, Application)
		void CloseApplication() override;
		void NewFile() override;
		void SaveFile() override;
		void OpenFile() override;
	REGISTER_INTERFACE_END(Application)

public:
	CApplication(QWidget *parent = Q_NULLPTR);
	~CApplication();
	void Init();
	void InitMenuBar();
	void SaveChanges(QString const& strPath);
	bool AskForSave();
	inline bool HasUnsaved() { return m_bUnsaved; }	

protected slots:
	void ActionHandler();
	void OnChanged();

private:
	QPlainTextEdit* m_pEditor;
	CBreakpointWindow* m_pBreakpointWindow;
	CConsole* m_pConsoleWindow;
	QMenuBar* m_pMenuBar;
	QString m_strWorkingPath;
	CApplicationManager* m_pManager;
	bool m_bUnsaved;
};
