#pragma once

#include "breakpoints_global.h"
#include <QWidget>

class QMenuBar;

extern "C" BREAKPOINTS_EXPORT void LoadPlugin(QMenuBar*);

class BREAKPOINTS_EXPORT CBreakpoints : public QWidget
{
public:
	CBreakpoints();

public:
	friend BREAKPOINTS_EXPORT void LoadPlugin(QMenuBar*);
};
