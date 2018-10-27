#pragma once

#include "core.h"
#include <QString>

class CORE_EXPORT IUnknown
{
public:
	static const QString UUID;
	virtual ~IUnknown();
};

class CORE_EXPORT IBreakpoint : public IUnknown
{
public:
	virtual void ToggleBreakpoint(int) = 0;
};

class CORE_EXPORT IConsole : public IUnknown
{
public:
	virtual void PrintMessage(QString const&) = 0;
	virtual void PrintError(QString const&) = 0;
	virtual void PrintWarning(QString const&) = 0;
	virtual void GetInput(QString const&) = 0;
};

class CORE_EXPORT ITextEditor : public IUnknown
{
public:
	virtual void AddText(QString const&) = 0;
	virtual void SetCursor(int, int) = 0;
};

class CORE_EXPORT IApplication : public IUnknown
{
public:
	virtual void CloseApplication() = 0;
	virtual void SaveFile() = 0;
	virtual void OpenFile() = 0;
	virtual void NewFile() = 0;
};