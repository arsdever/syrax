#pragma once

#include <QDockWidget>
#include "widgets_global.h"

class CDockWidgetTitleBar;
class QPaintEvent;
class QCloseEvent;

class WIDGETS_EXPORT CDockWidget : public QDockWidget
{

	Q_OBJECT

public:
	CDockWidget(const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = 0);
	CDockWidget(QWidget * parent = 0, Qt::WindowFlags flags = 0);

	void SetupUI();
	inline bool Focused() { return m_bFocused; }

protected:
	void closeEvent(QCloseEvent* pEvent) override;
	void paintEvent(QPaintEvent* pEvent) override;

private slots:
	void SetFocus();

signals:
	void SigFocusChanged(bool);

private:
	CDockWidgetTitleBar* m_pTitleBar;
	bool m_bFocused;
};

