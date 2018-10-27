#include "application.h"
#include <code_editor.h>
#include <bparea.h>
#include <bpwindow.h>
#include <dockwidget.h>
#include <decorations.h>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include "../console/console.h"
#include "../console/console_mgr.h"
#include <editormgr.h>

CApplication::CApplication(QWidget *parent)
	: QMainWindow(parent)
{
	Init();
	m_pManager = GetCore()->GetManager<CApplicationManager>();
	if (m_pManager == nullptr)
		return;

	m_pManager->RegisterInterface(&m_xApplication);
}

CApplication::~CApplication()
{
}

void CApplication::Init()
{
	QTabWidget* pCentralWidget = new QTabWidget();
	pCentralWidget->setTabsClosable(true);
	m_pBreakpointWindow = new CBreakpointWindow();
	m_pConsoleWindow = new CConsole();
	CEditorManager* pEditorManager = GetCore()->GetManager<CEditorManager>();

	if (pEditorManager != nullptr)
		pEditorManager->SetTabWidget(pCentralWidget);

	setCentralWidget(pCentralWidget);
	m_bUnsaved = false;

	CDockWidget* pBreakpointsDock = new CDockWidget("Breakpoints", this);
	pBreakpointsDock->setFloating(false);
	pBreakpointsDock->setAllowedAreas(Qt::AllDockWidgetAreas);
	pBreakpointsDock->setWidget(m_pBreakpointWindow);
	addDockWidget(Qt::RightDockWidgetArea, pBreakpointsDock);

	CDockWidget* pConsoleDock = new CDockWidget("Console", this);
	pConsoleDock->setFloating(false);
	pConsoleDock->setAllowedAreas(Qt::AllDockWidgetAreas);
	pConsoleDock->setWidget(m_pConsoleWindow);
	addDockWidget(Qt::BottomDockWidgetArea, pConsoleDock);

	InitMenuBar();
}

void CApplication::InitMenuBar()
{
	m_pMenuBar = new QMenuBar();
	setMenuBar(m_pMenuBar);
	QMenu* menu = nullptr;
	menu = menuBar()->addMenu("File");
	menu->addAction("New", this, SLOT(ActionHandler()), QKeySequence("CTRL+N"));
	menu->addAction("Save", this, SLOT(ActionHandler()), QKeySequence("CTRL+S"));
	menu->addAction("Open", this, SLOT(ActionHandler()), QKeySequence("CTRL+O"));
	menu->addSeparator();
	menu->addAction("Exit", this, SLOT(ActionHandler()), QKeySequence("CTRL+X"));
	menu = menuBar()->addMenu("View");
	menu->addAction("Breakpoint", this, SLOT(ActionHandler()));
	menu = menuBar()->addMenu("Breakpoint");
	menu->addAction("Toggle breakpoint", this, SLOT(ActionHandler()), QKeySequence("F9"));
	menu->addAction("Clear breakpoints", this, SLOT(ActionHandler()));
}

void CApplication::OnChanged()
{
	m_bUnsaved = true;
}

void CApplication::ActionHandler()
{
	QAction* pAction = dynamic_cast<QAction*>(sender());

	if (pAction == nullptr)
		return;

	QMenu* pMenu = dynamic_cast<QMenu*>(pAction->parent());
	if (pMenu->title() == "File")
	{/*
		if (pAction->text() == "New" && AskForSave())
		{
			m_pEditor->setPlainText("");
			return;
		}*/
		if (pAction->text() == "New")
		{
			CEditorManager* pManager = GetCore()->GetManager<CEditorManager>();
			if (pManager != nullptr)
				pManager->NewEditor("./tmp.txt");
			return;
		}
		if (pAction->text() == "Save")
		{
			m_pManager->SaveChanges();
		}
		if (pAction->text() == "Open" && AskForSave())
		{
			QString path = QFileDialog::getOpenFileName();
			if (path == "")
				return;

			m_strWorkingPath = path;
			CConsoleManager* pManager = GetCore()->GetManager<CConsoleManager>();
			if (pManager != nullptr)
				pManager->PrintInfoMessage(tr("Opening file from path \"%0\"").arg(path));

			QFile file(path);
			file.open(QIODevice::ReadOnly);
			if (file.isOpen())
			{
				QString content = file.readAll();
				m_pEditor->setPlainText(content);
			}
			else
			{
				pManager->PrintErrorMessage("Cannot open the file.");
			}

			m_bUnsaved = false;
			return;
		}
		if (pAction->text() == "Exit")
		{
			if (AskForSave())
				m_pManager->CloseApplication();
		}
	}
	else if (pMenu->title() == "View")
	{
		if (pAction->text() == "Breakpoint")
		{
			return;
		}
	}
	else if (pMenu->title() == "Breakpoint")
	{
		if (pAction->text() == "Toggle breakpoint")
		{
			CBreakpointManager* pManager = GetCore()->GetManager<CBreakpointManager>();

			if(pManager != nullptr)
				pManager->ToggleBreakpoint(m_pEditor->textCursor().blockNumber() + 1);
		}
		else if (pAction->text() == "Clear breakpoints")
		{
			return;
		}
	}
}

void CApplication::SaveChanges(QString const& strPath)
{
	CConsoleManager* pManager = GetCore()->GetManager<CConsoleManager>();
	if (pManager != nullptr)
		pManager->PrintInfoMessage("Application was saved.");
	return;
}

bool CApplication::AskForSave()
{
	if (!HasUnsaved())
		return true;

	CConsoleManager* pManager = GetCore()->GetManager<CConsoleManager>();
	if (pManager != nullptr)
		pManager->PrintWarningMessage("You have unsaved changes!");
	int answer = QMessageBox::question(this, "Unsaved changes", "You have an unsaved changes. Do you want save?", QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);

	if (answer == QMessageBox::Cancel)
		return false;
		
	if (answer == QMessageBox::Yes)
		m_pManager->SaveChanges();

	return true;
}


void CApplication::XApplication::CloseApplication()
{
	m_pThis->close();
}

void CApplication::XApplication::NewFile()
{

}

void CApplication::XApplication::SaveFile()
{

}

void CApplication::XApplication::OpenFile()
{

}