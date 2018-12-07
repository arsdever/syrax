#pragma once

#include "logger_global.h"

#include <core.h>

#include <QFile>

class LOGGER_EXPORT CLogger
{

	REGISTER_INTERFACE(CLogger, Logger)
		virtual void Info(QString const&) override;
		virtual void Warning(QString const&) override;
		virtual void Error(QString const&) override;
	REGISTER_INTERFACE_END(Logger)

public:
	CLogger();
	~CLogger();

private:
	QFile m_infoOutput;
	QFile m_warnOutput;
	QFile m_errorOutput;
};
