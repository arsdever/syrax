#pragma once

#include "editor_global.h"

#include <interfaces.h>
#include <macros.h>
#include <editor.h>

#include <QObject>

class QPlainTextEdit;
class QTabWidget;

class EDITOR_EXPORT CEditorManager : public QObject
{

	Q_OBJECT

	REGISTER_INTERFACE(CEditorManager, FileManipulator)
		virtual void Open(QStringList const&) override;
		virtual void CloseAll(int) override;
		virtual void SaveAll() override;
		virtual void Close(int) override;
		virtual void Save(QString const&) override;
		virtual void New() override;
	REGISTER_INTERFACE_END(FileManipulator)
	CEditorManager(QObject* pParent = nullptr);

public:
	~CEditorManager();

	CEditor* GetCurrentEditor() const;
	CEditor* GetEditor(QPlainTextEdit* pEditor) const;
	CEditor* GetEditorAt(int nIndex) const;
	
	void Close(int nIndex);

	void CloseEditor(CEditor* pEditor);
	void CloseEditor(QPlainTextEdit* pEditor);

	void SetCurrentEditor(int nIndex);
	void SetCurrentEditor(CEditor* pEditor);
	void SetCurrentEditor(QPlainTextEdit* pEditor);

	void SaveCurrent();
	void SaveAll();

	void SetTabWidget(QTabWidget* pTabWidget);
	QTabWidget* GetTabWidget() const;

	int GetTabIndex(CEditor* pEditor) const;
	int GetTabIndex(QPlainTextEdit* pEditor) const;

	static CEditorManager* GlobalInstance();

signals:
	void CurrentEditorChanged(int);

public slots:
	void OnClose(int nIndex);
	void OnCurrentChanged(int nIndex);

private:
	QList<CEditor*> m_lstEditors;
	int m_nCurrentEditor;
	QTabWidget* m_pTabWidget;
	static CEditorManager* s_pGlobalInstance;
};