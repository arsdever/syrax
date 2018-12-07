#pragma once

#include "application_global.h"

#include <core.h>

#include <QMainWindow>
#include <QList>

class QMenuBar;
class CDockWidget;

class APPLICATION_EXPORT CApplication : public QMainWindow
{
	Q_OBJECT

	REGISTER_INTERFACE(CApplication, Application)
		virtual void Close() override;
		virtual void AddDockWidget(QWidget*, QString const&, Qt::DockWidgetArea) override;
		virtual void RemoveDockWidget(QWidget*) override;
	REGISTER_INTERFACE_END(Application)

public:
	CApplication(QWidget *parent = Q_NULLPTR);
	~CApplication();

	void AttachPlugins();
	void Init();
	void InitMenuBar();

	void LoadPlugin(QString const& path);
	void UnloadPlugin(QString const& path);

protected:
	void closeEvent(QCloseEvent* pEvent) override;

protected slots:
	void ActionHandler(bool toggled = false);

private:
	QMenuBar* m_pMenuBar;
	QList<CDockWidget*> m_lstDocks;
	QList<QString> m_lstPlugins;
};
