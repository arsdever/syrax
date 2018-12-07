#include "editor.h"

#include <core.h>
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
		CallFunction<ILogger>(ILogger::InfoFunctor("Creating new editor tab."));
		m_bIsValid = false;
		return;
	}

	CallFunction<ILogger>(ILogger::InfoFunctor(QString("Opening \"%1\" file into new tab.").arg(strPath)));
	QString content = m_fCurrentFile.readAll();
	m_pCore->setPlainText(content);
	m_fCurrentFile.close();
	m_bUnsaved = false;
	SetTitle(GetFileName());
}

CEditor::~CEditor()
{
	delete m_pCore;
}

QString const& CEditor::GetTitle() const
{
	return m_strTitle;
}

void CEditor::SetTitle(QString const& strTitle)
{
	CallFunction<ILogger>(ILogger::InfoFunctor(QString("Changing title on tab \"%1\" to \"%2\"").arg(GetTitle()).arg(strTitle)));
	m_strTitle = strTitle;
	emit TitleChanged();
}

QString CEditor::GetFileName() const
{
	QString fname = QFileInfo(m_fCurrentFile).fileName();
	return fname == "" ? "New File" : fname;
}

QString CEditor::GetFilePath() const
{
	bool b = m_fCurrentFile.exists();
	return m_fCurrentFile.exists() ? QFileInfo(m_fCurrentFile).filePath() : "";
}

void CEditor::SetFilePath(QString const& strPath)
{
	CallFunction<ILogger>(ILogger::InfoFunctor(QString("Changing path on tab \"%1\" to \"%2\"").arg(GetTitle()).arg(strPath)));
	m_fCurrentFile.setFileName(strPath);
	SetTitle(QFileInfo(m_fCurrentFile).fileName());
	emit PathChanged();
}

bool CEditor::Save()
{
	CallFunction<ILogger>(ILogger::InfoFunctor(QString("Saving \"%1\"...").arg(GetTitle())));
	if (!IsUnsaved())
	{
		CallFunction<ILogger>(ILogger::WarningFunctor(QString("Tab is up to date.")));
		return true;
	}

	while (m_fCurrentFile.fileName() == "")
	{
		CallFunction<ILogger>(ILogger::InfoFunctor(QString("Tab path is not specified. Trying to get valid path...")));
		QString path = QFileDialog::getSaveFileName(this, "Save", "./");

		if (path == "")
		{
			CallFunction<ILogger>(ILogger::ErrorFunctor(QString("Operation canceled.")));
			return false;
		}

		m_fCurrentFile.setFileName(path);
	}

	if (m_fCurrentFile.exists() && !m_fCurrentFile.isWritable())
	{
		CallFunction<ILogger>(ILogger::ErrorFunctor(QString("File is read only.")));
		QMessageBox::warning(this, "Read only file", "There are no permissions for writing this file.");
		return false;
	}

	m_fCurrentFile.open(QIODevice::WriteOnly);
	if (!m_fCurrentFile.isOpen())
	{
		CallFunction<ILogger>(ILogger::ErrorFunctor(QString("Unknown error")));
		QMessageBox::critical(this, "Unknown error", "Cannot open file for writing.");
		return false;
	}

	m_fCurrentFile.write(m_pCore->toPlainText().toUtf8());
	m_bUnsaved = false;
	CallFunction<ILogger>(ILogger::InfoFunctor(QString("Saved.")));
	SetTitle(GetFileName());
	return true;
}

bool CEditor::AskForSave()
{
	CallFunction<ILogger>(ILogger::InfoFunctor(QString("Closing %1...").arg(GetTitle())));
	if (IsUnsaved())
	{
		QMessageBox::StandardButton answer = QMessageBox::StandardButton(QMessageBox::question(this, "Save before closing", "You have unsaved changes. Do you want to save before closing?", QMessageBox::Cancel, QMessageBox::No, QMessageBox::Save));
		switch (answer)
		{
		case QMessageBox::Save:
			CallFunction<ILogger>(ILogger::WarningFunctor(QString("Saving changes.").arg(GetTitle())));
			return Save();
		case QMessageBox::No:
			CallFunction<ILogger>(ILogger::WarningFunctor(QString("Skipping changes.").arg(GetTitle())));
			return true;
		case QMessageBox::Cancel:
			CallFunction<ILogger>(ILogger::ErrorFunctor(QString("Canceled.").arg(GetTitle())));
			return false;
		}
	}

	return true;
}

void CEditor::OnChange()
{
	m_bUnsaved = true;
}