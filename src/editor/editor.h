#pragma once

#include <interfaces.h>
#include <macros.h>
#include <QWidget>
#include <QFile>

class QPlainTextEdit;
class QString;

class CEditor : public QWidget
{

	Q_OBJECT

public:
	CEditor(QString const& strPath = "", QWidget* pParent = nullptr);
	~CEditor();

	QString GetFileName() const;
	QString GetFilePath() const;

	QPlainTextEdit* GetCoreWidget() { return m_pCore; }
	bool IsValid() const { return m_bIsValid; }
	bool IsUnsaved() const { return m_bUnsaved; }

	bool AskForSave();
	bool Save();

public slots:
	void OnChange();

private:
	QPlainTextEdit* m_pCore;
	QFile m_fCurrentFile;
	bool m_bIsValid;
	bool m_bUnsaved;
};