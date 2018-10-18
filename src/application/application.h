#pragma once

#include <QMainWindow>
#include <bpmanager.h>

class QPlainTextEdit;
class CBreakpointWindow;
class CConsole;

class CApplication : public QMainWindow
{
	Q_OBJECT

public:
	CApplication(QWidget *parent = Q_NULLPTR);
	~CApplication();
	void Init();
	void InitMenuBar();
	void SaveChanges();
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
	bool m_bUnsaved;
};
