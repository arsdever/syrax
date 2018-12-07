#include "core.h"
#include "interfaces.h"

static CCore instance;

CCore::CCore(QObject* pParent)
	: QObject(pParent)
{}

CCore::~CCore()
{
	for (QSet<IUnknown*> plgSet : m_mapPlugins)
	{
		for (IUnknown* plg : plgSet)
			delete plg;
	}
}

extern "C" CORE_EXPORT CCore* GetCore()
{
	return &instance;
}

void CCore::RegisterInterface(IUnknown* pInterface)
{
	if (pInterface == nullptr || m_mapPlugins[pInterface->GetUUID()].contains(pInterface))
		return;

	m_mapPlugins[pInterface->GetUUID()].insert(pInterface);
}

void CCore::UnregisterInterface(IUnknown * pInterface)
{
	if (pInterface == nullptr || !m_mapPlugins.contains(pInterface->GetUUID()) || !m_mapPlugins[pInterface->GetUUID()].contains(pInterface))
		return;
	
	m_mapPlugins.remove(pInterface->GetUUID());
}

QSet<IUnknown*> CCore::QueryInterface(QString const& strInterfaceUUID)
{
	if (m_mapPlugins.contains(strInterfaceUUID))
		return m_mapPlugins[strInterfaceUUID];

	return QSet<IUnknown*>();
}