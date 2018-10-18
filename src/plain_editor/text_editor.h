#pragma once

#include "plain_editor_global.h"
#include <QWidget>
#include <QList>

class QPlainTextEdit;

class PLAIN_EDITOR_EXPORT CTextEdit : public QWidget
{
	typedef QWidget Decorator;

public:
	CTextEdit(QWidget* pParent = nullptr);
	~CTextEdit();

	void AddDecoration(QWidget* pDecoration, Qt::Alignment eAlign);
	operator QPlainTextEdit*() { return m_pCoreWidget; }

private:
	QLayout* m_pLayout;
	QPlainTextEdit* m_pCoreWidget;
	QList<Decorator*> m_lstDecorators;
};

