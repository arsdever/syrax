#include "application_mgr.h"
#include <core.h>
#include <interfaces.h>

CApplicationManager::CApplicationManager(QObject* pParent)
	: CPluginManager(pParent)
{
}

CApplicationManager::~CApplicationManager()
{
}

void CApplicationManager::CloseApplication()
{
	for (auto it : m_lstPlugins)
	{
		IApplication* pApplication = dynamic_cast<IApplication*>((IUnknown*)it);
		if (pApplication == nullptr)
			continue;

		pApplication->CloseApplication();
	}
}

void CApplicationManager::SaveChanges()
{

}

void CApplicationManager::OpenFile()
{

}