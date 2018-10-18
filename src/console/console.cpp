#include "console.h"

#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QApplication>
#include <core.h>
#include "console_mgr.h"
#include <QKeyEvent>

CConsole::CConsole(QWidget* pParent)
	: QTextEdit(pParent)
{
	setReadOnly(true);
	m_pManager = GetCore()->GetManager<CConsoleManager>();

	if (m_pManager != nullptr)
		m_pManager->RegisterInterface(&m_xConsole);
}

void CConsole::keyPressEvent(QKeyEvent* pEvent)
{
	switch (pEvent->key())
	{
	case Qt::Key_Enter:
		ParseInputString();
		break;
	case Qt::Key_Backspace:
		if (m_strInputString == "")
			break;

		QTextEdit::keyPressEvent(pEvent);

		if (m_strInputString[m_strInputString.length() - 1] == '\n')
			m_strInputString.chop(2);
		else
			m_strInputString.chop(1);

		break;
	default:
		m_strInputString += pEvent->text();
		QTextEdit::keyPressEvent(pEvent);
	}
}

void CConsole::ParseInputString()
{
	// magic happens
}

void CConsole::XConsole::PrintMessage(QString const& str)
{
	m_pThis->append(tr("%0 : %1").arg("[INFO] ", 10).arg(str));
}

void CConsole::XConsole::PrintError(QString const& str)
{
	m_pThis->append(tr("%0 : %1").arg("[ERROR]", 10).arg(str));
}

void CConsole::XConsole::PrintWarning(QString const& str)
{
	m_pThis->append(tr("%0 : %1").arg("[WARN] ", 10).arg(str));
}

void CConsole::XConsole::GetInput(QString const& str)
{
	m_pThis->append(str);
}