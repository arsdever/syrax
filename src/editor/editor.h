#pragma once

#include <QFile>

class QPlainTextEdit;
class QString;

class CEditor
{
public:
	CEditor(QString const& strPath);
	~CEditor();

	QString GetFileName() const;
	QString GetFilePath() const;

	QWidget* GetCore() { return m_pCore; }

private:
	QWidget* m_pCore;
	QFile m_fCurrentFile;
};