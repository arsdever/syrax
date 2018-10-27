#pragma once

#include "editor_global.h"
#include <pluginmgr.h>
#include <editor.h>

class QPlainTextEdit;
class QTabWidget;

class EDITOR_EXPORT CEditorManager : public CPluginManager
{

	Q_OBJECT

public:
	CEditorManager(QObject* pParent = nullptr);
	~CEditorManager();

	CEditor* GetCurrentEditor() const;
	CEditor* GetEditor(QPlainTextEdit* pEditor) const;
	
	void NewEditor(QString const& strPath);

	void CloseEditor(int nIndex);
	void CloseEditor(CEditor* pEditor);
	void CloseEditor(QPlainTextEdit* pEditor);

	void SetCurrentEditor(int nIndex);
	void SetCurrentEditor(CEditor* pEditor);
	void SetCurrentEditor(QPlainTextEdit* pEditor);

	void SetTabWidget(QTabWidget* pTabWidget);

	int GetTabIndex(CEditor* pEditor) const;
	int GetTabIndex(QPlainTextEdit* pEditor) const;

signals:
	void CurrentEditorChanged(int);

private:
	QList<CEditor*> m_lstEditors;
	int m_nCurrentEditor;
	QTabWidget* m_pTabWidget;
};
