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
		virtual void New() override;
	
		virtual void Open(QStringList const&) override;

		virtual void Save(QString const& = "") override;
		virtual void SaveAs() override;
		virtual void SaveAll() override;

		virtual void Close(qint32 = -1) override;

		virtual bool AskForClose(qint32, EClosingType) override;
	REGISTER_INTERFACE_END(FileManipulator)
	CEditorManager(QObject* pParent = nullptr);

public:
	~CEditorManager();

	CEditor* GetCurrentEditor() const;
	qint32 GetCurrentEditorIndex() const;
	CEditor* GetEditor(QPlainTextEdit* pEditor) const;
	CEditor* GetEditorAt(int nIndex) const;

	void SetCurrentEditor(int nIndex);
	void SetCurrentEditor(CEditor* pEditor);
	void SetCurrentEditor(QPlainTextEdit* pEditor);

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
	void OnTitleUpdated();

private:
	QList<CEditor*> m_lstEditors;
	qint32 m_nCurrentEditor;
	QVector<void*> m_arrUntitledIndexes;
	QVector<void*>::iterator m_itLastUntitled;
	QTabWidget* m_pTabWidget;
	static CEditorManager* s_pGlobalInstance;
};