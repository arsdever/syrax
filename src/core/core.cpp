#include "core.h"
#include "interfaces.h"

static CCore instance;

CCore::CCore(QObject* pParent)
	: QObject(pParent)
{}

extern "C" CORE_EXPORT CCore* GetCore()
{
	return &instance;
}

void CCore::RegisterInterface(IUnknown* pInterface)
{
	if (pInterface != nullptr && !s_mapPlugins[pInterface->GetUUID()].contains(pInterface))
		s_mapPlugins[pInterface->GetUUID()].insert(pInterface);
}

QSet<IUnknown*> CCore::QueryInterface(QString const& strInterfaceUUID)
{
	if (s_mapPlugins.contains(strInterfaceUUID))
		return s_mapPlugins[strInterfaceUUID];

	return QSet<IUnknown*>();
}

QMap<QString, QSet<IUnknown*>> CCore::s_mapPlugins;