#pragma once

#include "mousestat.h"
#include <QWidget>

class QLabel;
class CTitleBarButton;
class QSpacerItem;
class QPaintEvent;
class CDockWidget;

class CDockWidgetTitleBar : public IMouseStatements
{

	Q_OBJECT

public:
	CDockWidgetTitleBar(QString const& strTitle = "Untitled", CDockWidget* pParent = nullptr);

	void SetupUI();
	inline QSize sizeHint() const override;
	inline QSize minimumSizeHint() const override;

	void SetTitle(QString const& strTitle); 
	void DrawButton(CTitleBarButton* pButton, QPaintEvent* pEvent);

private:
	void paintEvent(QPaintEvent* pEvent);

signals:
	void SigClose();

public slots:
	void UpdateIcon();
	void UpdateFocus(bool bFocused);

private:
	QLabel* m_pTitleText;
	QSpacerItem* m_pSpacer;
	CTitleBarButton* m_pCloseButton;
	CDockWidget* m_pParent;
	bool m_bFocused;
};

