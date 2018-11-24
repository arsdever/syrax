#include "editor.h"

#include <QString>
#include <QFileInfo>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QFileDialog>

CEditor::CEditor(QString const& strPath, QWidget* pParent)
	: QWidget(pParent)
	, m_pCore(new QPlainTextEdit())
	, m_fCurrentFile(strPath)
	, m_bIsValid(true)
	, m_bUnsaved(false)
{
	connect(m_pCore, SIGNAL(textChanged()), this, SLOT(OnChange()));
	m_fCurrentFile.open(QIODevice::ReadOnly | QIODevice::Text);
	if (!m_fCurrentFile.isOpen())
	{
		m_bIsValid = false;
		return;
	}

	QString content = m_fCurrentFile.readAll();
	m_pCore->setPlainText(content);
	m_fCurrentFile.close();
	m_bUnsaved = false;
}

CEditor::~CEditor()
{
	delete m_pCore;
}

QString CEditor::GetFileName() const
{
	QString fname = QFileInfo(m_fCurrentFile).fileName();
	return fname == "" ? "New File" : fname;
}

QString CEditor::GetFilePath() const
{
	QString fpath = QFileInfo(m_fCurrentFile).filePath();
	return fpath == "" ? "New File" : fpath;
}

bool CEditor::Save()
{
	if (!IsUnsaved())
		return true;

	while (m_fCurrentFile.fileName() == "")
	{
		QString path = QFileDialog::getSaveFileName(this, "Save", "./");

		if (path == "")
			return false;

		m_fCurrentFile.setFileName(path);
	}

	if (m_fCurrentFile.exists() && !m_fCurrentFile.isWritable())
	{
		QMessageBox::warning(this, "Read only file", "There are no permissions for writing this file.");
		return false;
	}

	m_fCurrentFile.open(QIODevice::WriteOnly);
	if (!m_fCurrentFile.isOpen())
	{
		QMessageBox::critical(this, "Unknown error", "Cannot open file for writing.");
		return false;
	}

	m_fCurrentFile.write(m_pCore->toPlainText().toUtf8());
	m_bUnsaved = false;
	return true;
}

bool CEditor::AskForSave()
{
	if (IsUnsaved())
	{
		QMessageBox::StandardButton answer = QMessageBox::StandardButton(QMessageBox::question(this, "Save before closing", "You have unsaved changes. Do you want to save before closing?", QMessageBox::Cancel, QMessageBox::No, QMessageBox::Save));
		switch (answer)
		{
		case QMessageBox::Save:
			return Save();
		case QMessageBox::No:
			return true;
		case QMessageBox::Cancel:
			return false;
		}
	}

	return true;
}

void CEditor::OnChange()
{
	m_bUnsaved = true;
}