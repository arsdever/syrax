#pragma once

#include <pluginmgr.h>
#include "application_global.h"

class APPLICATION_EXPORT CApplicationManager : public CPluginManager
{

	Q_OBJECT

public:
	CApplicationManager(QObject* pParent = nullptr);
	~CApplicationManager();

	void CloseApplication();
	void SaveChanges();
	void OpenFile();
};
