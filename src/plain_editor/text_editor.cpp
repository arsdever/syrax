#include "text_editor.h"

#include <QPlainTextEdit>
#include <decoration.h>

#include <QHBoxLayout>
#include <QVBoxLayout>

CTextEdit::CTextEdit(QWidget* pParent)
	: QWidget(pParent)
	, m_pCoreWidget(new QPlainTextEdit(this))
	, m_pLayout(new QHBoxLayout())
{
	m_pLayout->addWidget(m_pCoreWidget);
	m_pLayout->setContentsMargins(0, 0, 0, 0);
	m_pLayout->setSpacing(0);
	setLayout(m_pLayout);
}

CTextEdit::~CTextEdit()
{
}

void CTextEdit::AddDecoration(QWidget* pDecoration, Qt::Alignment eAlign)
{
	QLayout* pLayout = m_pLayout;

	switch (eAlign)
	{
	case Qt::AlignLeft:
		QHBoxLayout* pLayout = new QHBoxLayout();
		pLayout->addWidget(pDecoration);
		pLayout->addItem(pLayout);
		for (int i = 0; i < m_pLayout->count(); ++i)
			m_pLayout->removeItem(m_pLayout->itemAt(i));
		setLayout(m_pLayout);
		break;
	case Qt::AlignRight:
		m_pLayout = new QHBoxLayout();
		m_pLayout->addItem(pLayout);
		m_pLayout->addWidget(pDecoration);
		break;
	case Qt::AlignTop:
		m_pLayout = new QVBoxLayout();
		m_pLayout->addWidget(pDecoration);
		m_pLayout->addItem(pLayout);
		break;
	case Qt::AlignBottom:
		m_pLayout = new QVBoxLayout();
		m_pLayout->addItem(pLayout);
		m_pLayout->addWidget(pDecoration);
		break;
	}
	m_pLayout->setContentsMargins(0, 0, 0, 0);
	m_pLayout->setSpacing(0);

	m_lstDecorators.push_back(pDecoration);
	setLayout(m_pLayout);
}