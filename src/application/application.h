#pragma once

#include "application_global.h"

#include <interfaces.h>
#include <macros.h>

// QT includes
#include <QMainWindow>
#include <QSet>

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

protected slots:
	void ActionHandler(bool toggled = false);
	//void OnChanged();

private:
	QMenuBar* m_pMenuBar;
	QSet<CDockWidget*> m_setDocks;
};
