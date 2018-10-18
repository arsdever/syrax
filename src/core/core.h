#pragma once

#include "core_global.h"
#include <QList>
#include <QObject>

class CPluginManager;

class CORE_EXPORT CCore : public QObject
{
private:
	CCore()
		: QObject(nullptr)
	{

	}

public:
	void InstallManager(CPluginManager* pManager);
	void UninstallManager(CPluginManager* pManager);

	template <typename T>
	T*	GetManager() const
	{
		for (auto mgr : m_lstManagers)
		{
			T* pMgr = dynamic_cast<T*>(mgr);
			if (pMgr != nullptr)
				return pMgr;
		}

		return nullptr;
	}

	CORE_EXPORT friend CCore* GetCore();

private:
	QList<CPluginManager*> m_lstManagers;
	static CCore* s_pUniqueInstance;
};