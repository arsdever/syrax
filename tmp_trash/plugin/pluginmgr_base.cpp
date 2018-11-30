#include "pluginmgr_base.h"

CPluginManager::CPluginManager(QObject* pParent)
	: QObject(pParent)
{
	GetCore()->InstallManager(this);
}

CPluginManager::~CPluginManager()
{
	GetCore()->UninstallManager(this);
}

void CPluginManager::RegisterInterface(IUnknown* pPlugin)
{
	if (m_lstPlugins.indexOf(pPlugin) < 0)
		m_lstPlugins.append(pPlugin);
}