#include "application.h"

#include <dockwidget.h>
#include <editormgr.h>

#include <QMenuBar>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QLibrary>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QCloseEvent>

#include <QListWidget>
#include <QLayout>
#include <QPushButton>

QMenu* FindMenuByTitle(QMenuBar* pMenuBar, QString const& strTitle);
QMenu* FindMenuByTitle(QMenu* pMenu, QString const& strTitle);
QAction* FindActionByTitle(QMenuBar* pMenuBar, QString const& strTitle);
QAction* FindActionByTitle(QMenu* pMenu, QString const& strTitle);

CApplication::CApplication(QWidget *parent)
	: QMainWindow(parent)
{
	Init();
	AttachPlugins();
	resize(600, 400);
}

CApplication::~CApplication()
{
	delete CEditorManager::instance();
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
	QTabWidget* tabWidget = CEditorManager::instance()->GetTabWidget();

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
	menu->addAction("Save as...", this, SLOT(ActionHandler()), QKeySequence("CTRL+SHIFT+S"));
	menu->addAction("Save all", this, SLOT(ActionHandler()));
	menu->addSeparator();
	menu->addAction("Load plugin", this, SLOT(ActionHandler()));
	menu->addAction("Unload plugin", this, SLOT(ActionHandler()));
	menu->addSeparator();
	menu->addAction("Close", this, SLOT(ActionHandler()), QKeySequence("CTRL+X"));
	menu->addAction("Close All", this, SLOT(ActionHandler()), QKeySequence("CTRL+ALT+X"));
	menu->addAction("Exit", this, SLOT(ActionHandler()), QKeySequence("ALT+F4"));
}

void CApplication::LoadPlugin(QString const& path)
{
	QString plgName = QFileInfo(path).fileName().remove(QRegExp("\.dll$"));
	CallFunction<ILogger>(ILogger::InfoFunctor(QString("Loading plugin \"%1\"...").arg(plgName)));
	if (m_lstPlugins.contains(plgName))
	{
		CallFunction<ILogger>(ILogger::WarningFunctor(QString("Plugin is already loaded.")));
		return;
	}

	typedef void(*Initializer)(QMenuBar*);
	Initializer initializer = (Initializer)QLibrary::resolve(plgName, "LoadPlugin");

	if (initializer == nullptr)
	{
		CallFunction<ILogger>(ILogger::ErrorFunctor(QString("Failed to load plugin. Plugin must provide \"LoadPlugin(QMenuBar*)\" interface.")));
		return;
	}

	m_lstPlugins.push_back(plgName);
	initializer(menuBar());
}

void CApplication::UnloadPlugin(QString const& path)
{
	QString plgName = QFileInfo(path).fileName().remove(QRegExp("\.dll$"));
	CallFunction<ILogger>(ILogger::InfoFunctor(QString("Unloading plugin \"%1\"...").arg(plgName)));
	if (!m_lstPlugins.contains(plgName))
	{
		CallFunction<ILogger>(ILogger::ErrorFunctor(QString("Could not allocate plugin.")));
		return;
	}

	typedef void(*Initializer)(QMenuBar*);
	Initializer unloader = (Initializer)QLibrary::resolve(plgName, "UnloadPlugin");

	if (unloader == nullptr)
	{
		CallFunction<ILogger>(ILogger::ErrorFunctor(QString("Failed to unload plugin. Plugin must provide \"UnloadPlugin(QMenuBar*)\" interface.")));
		return;
	}

	m_lstPlugins.removeOne(plgName);
	unloader(menuBar());
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
		else if (pAction->text() == "Save as...")
			CallFunction<IFileManipulator>(IFileManipulator::SaveAsFunctor());
		else if (pAction->text() == "Save all")
			CallFunction<IFileManipulator>(IFileManipulator::SaveAllFunctor());
		else if (pAction->text() == "Open")
			CallFunction<IFileManipulator>(IFileManipulator::OpenFunctor(paths));
		else if (pAction->text() == "Load plugin")
		{
			QString path = QFileDialog::getOpenFileName(this, "Choose plugin file", ".", "Dynamic link library (*.dll)");
			if (path == "")
				return;

			LoadPlugin(path);
		}
		else if (pAction->text() == "Unload plugin")
		{
			QDialog* pDialog = new QDialog();
			pDialog->setLayout(new QVBoxLayout());
			QListWidget* list = new QListWidget();
			QPushButton* closeBtn = new QPushButton("Ok");
			pDialog->layout()->addWidget(list);
			pDialog->layout()->addWidget(closeBtn);
			connect(closeBtn, SIGNAL(clicked()), pDialog, SLOT(close()));
			list->addItems(m_lstPlugins);

			pDialog->exec();

			for (QListWidgetItem* item : list->selectedItems())
				UnloadPlugin(item->text());

			delete pDialog;
		}
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
		for(QString plgPath : m_lstPlugins)
		{
			UnloadPlugin(plgPath);
		}
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
	m_pThis->m_lstDocks.push_back(pDock);
}

void CApplication::XApplication::RemoveDockWidget(QWidget* pWidget)
{
	for (CDockWidget* pDock : m_pThis->m_lstDocks)
	{
		if (pDock->widget() != pWidget)
			continue;

		pDock->widget()->setParent(nullptr);
		pDock->setWidget(nullptr);
		m_pThis->removeDockWidget(pDock);
		m_pThis->m_lstDocks.removeOne(pDock);
		delete pDock;
		break;
	}
}

QMenu* FindMenuByTitle(QMenuBar* pMenuBar, QString const& strTitle)
{
	QList<QMenu*> menus = pMenuBar->findChildren<QMenu*>();
	for (QMenu* menu : menus)
	{
		if (menu->title() == strTitle)
			return menu;
	}

	for (QMenu* menu : menus)
	{
		QMenu* pMenu;
		if (pMenu = FindMenuByTitle(menu, strTitle))
			return pMenu;
	}

	return nullptr;
}

QMenu* FindMenuByTitle(QMenu* pMenu, QString const& strTitle)
{
	QList<QMenu*> menus = pMenu->findChildren<QMenu*>();
	for (QMenu* menu : menus)
	{
		if (menu->title() == strTitle)
			return menu;
	}

	return nullptr;
}

QAction* FindActionByTitle(QMenuBar* pMenuBar, QString const& strTitle)
{
	QList<QAction*> actions = pMenuBar->findChildren<QAction*>();
	for (QAction* action : actions)
	{
		if (action->text() == strTitle)
			return action;
	}

	QList<QMenu*> menus = pMenuBar->findChildren<QMenu*>();
	for (QMenu* menu : menus)
	{
		QAction* pAction;
		if (pAction = FindActionByTitle(menu, strTitle))
			return pAction;
	}

	return nullptr;
}

QAction* FindActionByTitle(QMenu* pMenu, QString const& strTitle)
{
	QList<QAction*> actions = pMenu->findChildren<QAction*>();
	for (QAction* action : actions)
	{
		if (action->text() == strTitle)
			return action;
	}

	return nullptr;
}
