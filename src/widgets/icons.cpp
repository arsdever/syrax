#include "icons.h"

#include <QPainter>

QMap<QString, CIconContainer::Creator> CIconContainer::s_mapIconProviders = {
	{"close", CIconContainer::CloseIcon}
};

QPixmap CIconContainer::CloseIcon(QColor cColor, int eSize)
{
	QPixmap icon = QPixmap::fromImage(QImage(QString(":/icons/Resources/icons_%1x%1/close.png").arg(eSize)));

	if (icon.isNull())
		return QPixmap();

	QPainter painter(&icon);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.setBrush(cColor);
	painter.drawRect(-1, -1, icon.width() + 1, icon.height() + 1);

	return icon;
}
QPixmap CIconContainer::BreakpointIcon(QColor cColor, int eSize)
{
	QPixmap icon(eSize, eSize);
	icon.fill(Qt::transparent);
	QPainter painter(&icon);
	painter.setBrush(QColor(168, 0, 0));
	painter.setPen(QPen(cColor, 2));
	painter.drawEllipse(icon.rect().adjusted(1, 1, -1, -1));
	return icon;
}

QPixmap CIconContainer::ProvideIcon(QString const& strIconName, QColor cColor, int eSize)
{
	if (s_mapIconProviders.constFind(strIconName) == s_mapIconProviders.cend())
		return QPixmap();

	return s_mapIconProviders[strIconName](cColor, eSize);
}
