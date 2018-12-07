#pragma once

#include <core.h>

class IBreakpoints : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() const override { return UUID; }

	virtual bool ToggleBreakpoint(quint32) = 0;
};