#pragma once

#include <QWidget>
#include "mousestat.h"

class QPaintEvent;
class QEvent;
class QMouseEvent;
class QPixmap;

class CPushButton : public IMouseStatements
{

	Q_OBJECT

public:
	CPushButton(QWidget* pParent = nullptr);

	void SetIcon(QString const& iIcon);

	virtual inline bool HasBorder() { return false; }
	inline QString IconName() { return m_strIcon; }

protected:
	void paintEvent(QPaintEvent* pEvent);

private:
	QString m_strIcon;
};

