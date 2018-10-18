#pragma once

#include <QColor>
#include <QPixmap>
#include <QMap>

class CIconContainer
{
private:
	CIconContainer() {}

public:
	typedef QPixmap (*Creator)(QColor, int);

public:
	static QPixmap CloseIcon(QColor cColor = QColor(0, 0, 0), int eSize = 8);
	static QPixmap BreakpointIcon(QColor cColor = QColor(0, 0, 0), int eSize = 8);

	static QPixmap ProvideIcon(QString const& strIconName, QColor cColor = QColor(0, 0, 0), int eSize = 8);

private:
	static QMap<QString, Creator> s_mapIconProviders;
};

