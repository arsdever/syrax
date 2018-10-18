#include "pushbutton.h"
#include "icons.h"
#include "settings.h"

#include <QEvent>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

CPushButton::CPushButton(QWidget* pParent)
	: IMouseStatements(pParent)
	, m_strIcon("")
{
}

void CPushButton::paintEvent(QPaintEvent* pEvent)
{
	QPainter paint(this);
	QColor clr = palette().color(QPalette::ButtonText);

	if (HasBorder())
		paint.setPen(palette().color(QPalette::ButtonText));
	else
		paint.setPen(Qt::NoPen);

	QPixmap icon;

	if (IsHover())
		if (IsPressed())
		{
			paint.setBrush(CSettings::GetGlobalDefaultSettings()->button.pressed);
			icon = CIconContainer::ProvideIcon(IconName(), CSettings::GetGlobalDefaultSettings()->button.text_icon.pressed);
		}
		else
		{
			paint.setBrush(CSettings::GetGlobalDefaultSettings()->button.hover);
			icon = CIconContainer::ProvideIcon(IconName(), CSettings::GetGlobalDefaultSettings()->button.text_icon.hover);
		}
	else
	{
		paint.setBrush(CSettings::GetGlobalDefaultSettings()->button.normal);
		icon = CIconContainer::ProvideIcon(IconName(), CSettings::GetGlobalDefaultSettings()->button.text_icon.normal);
	}

	paint.drawRect(rect());

	if (!icon.isNull())
		paint.drawPixmap((rect().width() - icon.width()) / 2, (rect().height() - icon.height()) / 2, icon);
}

void CPushButton::SetIcon(QString const& iIcon)
{
	m_strIcon = iIcon;
}