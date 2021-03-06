#pragma once

#include "core_global.h"
#include <QObject>
#include <QMap>
#include <QSet>

class IUnknown;

class CORE_EXPORT CCore : public QObject
{
public:
	CCore(QObject* pParent = nullptr);
	virtual ~CCore();

	void RegisterInterface(IUnknown* pInterface);
	void UnregisterInterface(IUnknown* pInterface);

	template <typename T>
	QSet<T*>  QueryInterface();

	QSet<IUnknown*> QueryInterface(QString const& strInterfaceUUID);

private:
	QMap<QString, QSet<IUnknown*>> m_mapPlugins;
};

#include "core_impl.h"

extern "C" CORE_EXPORT CCore* GetCore();