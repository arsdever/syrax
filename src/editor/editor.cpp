#include "editor.h"

#include <QPlainTextEdit>
#include <QString>
#include <QFileInfo>
#include <decorations.h>
#include <textedit>

CEditor::CEditor(QString const& strPath)
	: m_pCore(new CBreakpointDecoration(new CLineNumberDecoration(new CPublicPlainTextEdit())))
	, m_fCurrentFile(strPath)
{
}

CEditor::~CEditor()
{
}

QString CEditor::GetFileName() const
{
	QFileInfo fileInfo(m_fCurrentFile);
	return fileInfo.fileName();
}

QString CEditor::GetFilePath() const
{
	QFileInfo fileInfo(m_fCurrentFile);
	return fileInfo.filePath();
}