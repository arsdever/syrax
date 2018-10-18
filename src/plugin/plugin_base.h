#pragma once

#include "plugin_global.h"

class CPluginManager;

class PLUGIN_EXPORT CPlugin
{
public:
	CPlugin();
	virtual ~CPlugin();

	CPluginManager* GetManager();

private:
	CPluginManager* m_pManager;
};

