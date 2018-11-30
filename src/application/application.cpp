#include "application.h"

#include <core.h>
#include <dockwidget.h>
#include <editormgr.h>
#include <manipulators.h>

#include <QMenuBar>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QLibrary>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QCloseEvent>

CApplication::CApplication(QWidget *parent)
	: QMainWindow(parent)
{
	GetCore()->RegisterInterface(&m_xApplication);
	Init();
	AttachPlugins();
	resize(600, 400);
}

CApplication::~CApplication()
{
}

void CApplication::AttachPlugins()
{
	QFile pluginList("plugin_list.json");
	pluginList.open(QIODevice::ReadOnly);
	if (!pluginList.isOpen())
		return;

	QString data = pluginList.readAll();
	QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
	QJsonArray arr = doc.array();
	for (auto plg : arr)
	{
		LoadPlugin(plg.toString()
#ifdef Debug
			+ 'd'
#endif
		);
	}
}

void CApplication::Init()
{
	QTabWidget* tabWidget = CEditorManager::GlobalInstance()->GetTabWidget();

	Q_CHECK_PTR(tabWidget);
	setCentralWidget(tabWidget);
	InitMenuBar();
}

void CApplication::InitMenuBar()
{
	m_pMenuBar = new QMenuBar();
	setMenuBar(m_pMenuBar);
	QMenu* menu = nullptr;
	menu = menuBar()->addMenu("File");
	menu->addAction("New", this, SLOT(ActionHandler()), QKeySequence("CTRL+N"));
	menu->addAction("Open", this, SLOT(ActionHandler()), QKeySequence("CTRL+O"));
	menu->addAction("Save", this, SLOT(ActionHandler()), QKeySequence("CTRL+S"));
	menu->addAction("Save As...", this, SLOT(ActionHandler()), QKeySequence("CTRL+SHIFT+S"));
	menu->addAction("Save All", this, SLOT(ActionHandler()));
	menu->addSeparator();
	menu->addAction("Close", this, SLOT(ActionHandler()), QKeySequence("CTRL+X"));
	menu->addAction("Close All", this, SLOT(ActionHandler()), QKeySequence("CTRL+ALT+X"));
	menu->addAction("Exit", this, SLOT(ActionHandler()), QKeySequence("ALT+F4"));
}

void CApplication::LoadPlugin(QString const& path)
{
	CallFunction<ILogger>(ILogger::InfoFunctor(QString("Loading plugin %1").arg(path)));

	typedef void(*Initializer)(QMenuBar*);
	Initializer initializer = (Initializer)QLibrary::resolve(path, "LoadPlugin");

	if (initializer == nullptr)
	{
		CallFunction<ILogger>(ILogger::ErrorFunctor(QString("Loading plugin %1").arg(path)));
		return;
	}

	initializer(menuBar());
}

void CApplication::ActionHandler(bool toggled)
{
	QAction* pAction = dynamic_cast<QAction*>(sender());

	if (pAction == nullptr)
		return;

	QMenu* pMenu = dynamic_cast<QMenu*>(pAction->parent());
	if (pMenu->title() == "File")
	{
		if (pAction->text() == "Exit")
		{
			close();
			return;
		}

		QStringList paths;
		if (pAction->text() == "Open")
			paths = QFileDialog::getOpenFileNames();

		if (pAction->text() == "New")
			CallFunction<IFileManipulator>(IFileManipulator::NewFunctor());
		else if (pAction->text() == "Save")
			CallFunction<IFileManipulator>(IFileManipulator::SaveFunctor());
		else if (pAction->text() == "Save As...")
			CallFunction<IFileManipulator>(IFileManipulator::SaveAsFunctor());
		else if (pAction->text() == "Save All")
			CallFunction<IFileManipulator>(IFileManipulator::SaveAllFunctor());
		else if (pAction->text() == "Open")
			CallFunction<IFileManipulator>(IFileManipulator::OpenFunctor(paths));
		else if (pAction->text() == "Close")
			CallFunctionAndReturn<IFileManipulator>(IFileManipulator::AskForCloseFunctor());
		else if (pAction->text() == "Close All")
			CallFunctionAndReturn<IFileManipulator>(IFileManipulator::AskForCloseFunctor(-1, IFileManipulator::All));
	}
}

void CApplication::XApplication::Close()
{
	m_pThis->close();
}

void CApplication::closeEvent(QCloseEvent* pEvent)
{
	Q_UNUSED(pEvent)
	bool result = true;
	QMap<void*, bool> results = CallFunctionAndReturn<IFileManipulator>(IFileManipulator::AskForCloseFunctor(-1, IFileManipulator::All));
	for (bool b : results)
		result &= b;

	if (!result)
		pEvent->ignore();
	else
	{
		pEvent->accept();
	}
}

void CApplication::XApplication::AddDockWidget(QWidget* pWidget, QString const& strTitle, Qt::DockWidgetArea eArea)
{
	CDockWidget* pDock = new CDockWidget(strTitle, m_pThis);
	pDock->setWidget(pWidget);
	pDock->setAllowedAreas(Qt::AllDockWidgetAreas);
	switch (eArea)
	{
	case Qt::NoDockWidgetArea:
		pDock->setFloating(true);
		m_pThis->addDockWidget(Qt::LeftDockWidgetArea, pDock);
		break;
	default:
		m_pThis->addDockWidget(eArea, pDock);
	}
	m_pThis->m_setDocks.insert(pDock);
}

void CApplication::XApplication::RemoveDockWidget(QWidget* pWidget)
{
	for (CDockWidget* pDock : m_pThis->m_setDocks)
	{
		if (pDock->widget() != pWidget)
			continue;

		pDock->widget()->setParent(nullptr);
		pDock->setWidget(nullptr);
		m_pThis->removeDockWidget(pDock);
		m_pThis->m_setDocks.remove(pDock);
		delete pDock;
		break;
	}
}