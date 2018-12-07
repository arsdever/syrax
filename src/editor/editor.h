#pragma once

#include <core.h>
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

	void SetFilePath(QString const& strPath);

	QPlainTextEdit* GetCoreWidget() { return m_pCore; }
	bool IsValid() const { return m_bIsValid; }
	bool IsUnsaved() const { return m_bUnsaved; }

	QString const& GetTitle() const;
	void SetTitle(QString const& strTitle);

	bool AskForSave();
	bool Save();

signals:
	void TitleChanged();
	void PathChanged();

public slots:
	void OnChange();

private:
	QPlainTextEdit* m_pCore;
	QFile m_fCurrentFile;
	QString m_strTitle;
	bool m_bIsValid;
	bool m_bUnsaved;
};