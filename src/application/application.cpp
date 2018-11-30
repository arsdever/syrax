#include "application.h"

#include <core.h>
#include <dockwidget.h>

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
	typedef QTabWidget* (*Widget)();

#ifdef Debug
	Widget widgetGetter = (Widget)QLibrary::resolve("editord", "Widget");
#elif Release
	Widget widgetGetter = (Widget)QLibrary::resolve("editor", "Widget");
#endif

	Q_CHECK_PTR(widgetGetter);
	setCentralWidget(widgetGetter());

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
	typedef void(*Initializer)(QMenuBar*);
	Initializer initializer = (Initializer)QLibrary::resolve(path, "LoadPlugin");

	if (initializer == nullptr)
		return;

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

		QSet<IFileManipulator*> plgSet = GetCore()->QueryInterface<IFileManipulator>();
		for (IFileManipulator* plg : plgSet)
		{
			if (pAction->text() == "New")
				plg->New();
			else if (pAction->text() == "Save")
				plg->Save();
			else if (pAction->text() == "Save As...")
				plg->SaveAs();
			else if (pAction->text() == "Save All")
				plg->SaveAll();
			else if (pAction->text() == "Open")
				plg->Open(paths);
			else if (pAction->text() == "Close")
				plg->AskForClose();
			else if (pAction->text() == "Close All")
				plg->AskForClose(-1, IFileManipulator::All);
		}
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
	QSet<IFileManipulator*> plgList = GetCore()->QueryInterface<IFileManipulator>();
	for (IFileManipulator* plg : plgList)
	{
		result &= plg->AskForClose(-1, IFileManipulator::All);
	}

	if (!result)
		pEvent->ignore();
	else
	{
		for (IFileManipulator* plg : plgList)
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