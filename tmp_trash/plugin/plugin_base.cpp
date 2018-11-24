#include "plugin_base.h"

CPlugin::CPlugin()
{
}

CPlugin::~CPlugin()
{
}

CPluginManager* CPlugin::GetManager()
{
	return m_pManager;
}
