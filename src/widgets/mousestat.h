#pragma once

#include <QWidget>

class QEvent;
class QMouseEvent;

class IMouseStatements : public QWidget
{

	Q_OBJECT

public:
	IMouseStatements(QWidget* pParent = nullptr);

	inline bool IsHover() { return m_bHover; }
	inline bool IsPressed() { return m_bPressed; }

signals:
	void SigHoverStateChanged(bool);
	void SigPressStateChanged(bool);
	void SigClicked();

protected:
	void enterEvent(QEvent* pEvent);
	void leaveEvent(QEvent* pEvent);
	void mousePressEvent(QMouseEvent* pEvent);
	void mouseReleaseEvent(QMouseEvent* pEvent);

protected:
	bool m_bHover;
	bool m_bPressed;
};

