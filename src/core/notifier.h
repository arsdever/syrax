#pragma once

#include <QObject>

class INotifier
{
public:
	inline bool IsNotificationsBlocked() { return m_bIsBlocked; }

	inline bool BlockNotifications()
	{
		bool bOld = IsNotificationsBlocked();
		m_bIsBlocked = true;
		return bOld;
	}

	inline bool UnblockNotifications()
	{
		bool bOld = IsNotificationsBlocked();
		m_bIsBlocked = false;
		return bOld;
	}

	virtual void Notify() = 0;

private:
	bool m_bIsBlocked;
};

