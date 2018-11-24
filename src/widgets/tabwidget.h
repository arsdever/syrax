#pragma once

#include "widgets_global.h"
#include <QTabWidget>

class WIDGETS_EXPORT CTabWidget : public QTabWidget
{
public:
	CTabWidget(QWidget* pParent = nullptr);
	~CTabWidget();
};

