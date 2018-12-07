#include "logger.h"

#include <QFile>
#include <QDateTime>

static CLogger* instance;

CLogger::CLogger()
	: m_infoOutput(QString("./info%1.log").arg(QDateTime::currentDateTime().toString("dd_MM_yyyy")))
	, m_warnOutput(QString("./info%1.log").arg(QDateTime::currentDateTime().toString("dd_MM_yyyy")))
	, m_errorOutput(QString("./info%1.log").arg(QDateTime::currentDateTime().toString("dd_MM_yyyy")))
{
	m_infoOutput.open(QIODevice::WriteOnly | QIODevice::Append);
	m_warnOutput.open(QIODevice::WriteOnly | QIODevice::Append);
	m_errorOutput.open(QIODevice::WriteOnly | QIODevice::Append);
}

CLogger::~CLogger()
{
	m_infoOutput.close();
	m_warnOutput.close();
	m_errorOutput.close();
}

void CLogger::XLogger::Info(QString const& msg)
{
	m_pThis->m_infoOutput.write(QString("{%1} [%2] : %3\n").arg(QDateTime::currentDateTime().toString("hh:mm:ss"), 8).arg("INFO", 8).arg(msg).toLatin1());
	m_pThis->m_infoOutput.flush();
}

void CLogger::XLogger::Warning(QString const& msg)
{
	m_pThis->m_infoOutput.write(QString("{%1} [%2] : %3\n").arg(QDateTime::currentDateTime().toString("hh:mm:ss"), 8).arg("WARNING", 8).arg(msg).toLatin1());
	m_pThis->m_infoOutput.flush();
}

void CLogger::XLogger::Error(QString const& msg)
{
	m_pThis->m_infoOutput.write(QString("{%1} [%2] : %3\n").arg(QDateTime::currentDateTime().toString("hh:mm:ss"), 8).arg("ERROR", 8).arg(msg).toLatin1());
	m_pThis->m_infoOutput.flush();
}

class QMenuBar;
extern "C" LOGGER_EXPORT void LoadPlugin(QMenuBar*)
{
	instance = new CLogger();
}

extern "C" LOGGER_EXPORT void UnloadPlugin(QMenuBar*)
{
	delete instance;
}