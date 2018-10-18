#include "titlebar.h"
#include "icons.h"
#include "titlebarbutton.h"
#include "settings.h"
#include "dockwidget.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QSize>
#include <QSpacerItem>
#include <QPaintEvent>
#include <QPainter>
#include <QTextFormat>

CDockWidgetTitleBar::CDockWidgetTitleBar(QString const& strTitle, CDockWidget* pParent)
	: IMouseStatements(pParent)
	, m_pParent(pParent)
	, m_pCloseButton(new CTitleBarButton(this))
	, m_pSpacer(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed))
	, m_pTitleText(new QLabel(strTitle))
	, m_bFocused(false)
{
	connect(pParent, SIGNAL(paletteChanged()), this, SLOT(UpdateIcon));
	connect(pParent, SIGNAL(SigFocusChanged(bool)), this, SLOT(UpdateFocus(bool)));
	SetupUI();
}

void CDockWidgetTitleBar::SetupUI()
{
	m_pCloseButton->setMinimumSize(16, 16);
	connect(m_pCloseButton, SIGNAL(SigClicked()), this, SIGNAL(SigClose()));
	connect(this, SIGNAL(paletteChanged()), this, SLOT(UpdateIcon()));
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(4, 4, 4, 4);
	layout->addWidget(m_pTitleText);
	layout->addSpacerItem(m_pSpacer);
	layout->addWidget(m_pCloseButton);

	setLayout(layout);
	UpdateIcon();
}

void CDockWidgetTitleBar::paintEvent(QPaintEvent* pEvent)
{
	QPainter ptr(this);

	if (m_bFocused)
	{
		ptr.fillRect(rect(), CSettings::GetGlobalDefaultSettings()->dockwidget.backgroundfocus);
		ptr.fillRect(m_pSpacer->geometry().adjusted(4, 4, -4, -4), QBrush(CSettings::GetGlobalDefaultSettings()->dockwidget.foregroundfocus, CSettings::GetGlobalDefaultSettings()->dockwidget.handlerstyle));
	}
	else
	{
		ptr.fillRect(rect(), CSettings::GetGlobalDefaultSettings()->dockwidget.background);
		ptr.fillRect(m_pSpacer->geometry().adjusted(4, 4, -4, -4), QBrush(CSettings::GetGlobalDefaultSettings()->dockwidget.foreground, CSettings::GetGlobalDefaultSettings()->dockwidget.handlerstyle));
	}

}

void CDockWidgetTitleBar::SetTitle(QString const& strTitle)
{
	m_pTitleText->setText(strTitle);
}

void CDockWidgetTitleBar::UpdateIcon()
{
	m_pCloseButton->SetIcon("close");
}

QSize CDockWidgetTitleBar::sizeHint() const
{
	return QSize(24, 24);
}

QSize CDockWidgetTitleBar::minimumSizeHint() const
{
	return QSize(qMax(m_pTitleText->minimumWidth(), m_pCloseButton->minimumWidth()), qMax(m_pTitleText->minimumHeight(), m_pCloseButton->minimumHeight()));
}

void CDockWidgetTitleBar::UpdateFocus(bool bFocused)
{
	m_bFocused = bFocused;

	QPalette textPalette = m_pTitleText->palette();
	if (m_bFocused)
		textPalette.setColor(m_pTitleText->foregroundRole(), CSettings::GetGlobalDefaultSettings()->dockwidget.foregroundfocus);
	else
		textPalette.setColor(m_pTitleText->foregroundRole(), CSettings::GetGlobalDefaultSettings()->dockwidget.foreground);

	m_pTitleText->setPalette(textPalette);

	update();
}

void CDockWidgetTitleBar::DrawButton(CTitleBarButton* pButton, QPaintEvent* pEvent)
{
	if (pButton == m_pCloseButton)
	{

		if (m_pParent->Focused())
			return pButton->DefaultPaintEvent(pEvent);

		QPainter paint(pButton);
		QColor clr = palette().color(QPalette::ButtonText);

		if (pButton->HasBorder())
			paint.setPen(palette().color(QPalette::ButtonText));
		else
			paint.setPen(Qt::NoPen);

		QPixmap icon;

		if (pButton->IsHover())
			if (pButton->IsPressed())
			{
				paint.setBrush(CSettings::GetGlobalDefaultSettings()->button.pressed);
				icon = CIconContainer::ProvideIcon(pButton->IconName(), CSettings::GetGlobalDefaultSettings()->button.text_icon.pressed);
			}
			else
			{
				paint.setBrush(CSettings::GetGlobalDefaultSettings()->button.normal);
				icon = CIconContainer::ProvideIcon(pButton->IconName(), CSettings::GetGlobalDefaultSettings()->button.text_icon.normal);
			}
		else
		{
			paint.setBrush(CSettings::GetGlobalDefaultSettings()->button.hover);
			icon = CIconContainer::ProvideIcon(pButton->IconName(), CSettings::GetGlobalDefaultSettings()->button.text_icon.hover);
		}

		paint.drawRect(pEvent->rect());

		if (!icon.isNull())
			paint.drawPixmap((pEvent->rect().width() - icon.width()) / 2, (pEvent->rect().height() - icon.height()) / 2, icon);
	}
}