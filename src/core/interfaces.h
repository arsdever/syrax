#pragma once

#include "core.h"
#include <QString>

class CORE_EXPORT IUnknown
{
public:
	virtual QString const& GetUUID() = 0;
	virtual ~IUnknown() = 0;
};

class CORE_EXPORT IBreakpoint : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() { return UUID; }
	virtual void ToggleBreakpoint(unsigned int) = 0;
};

class CORE_EXPORT ITextEditor : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() { return UUID; }
	virtual void AddText(QString const&) = 0;
	virtual void SetCursor(int, int) = 0;
};

class CORE_EXPORT IWidgetProvider : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() { return UUID; }
	virtual QWidget* GetWidget(QString const&) = 0;
};

class CORE_EXPORT IApplication : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() { return UUID; }
	virtual void Close() = 0;
	virtual void AddDockWidget(QWidget*, QString const&, Qt::DockWidgetArea = Qt::NoDockWidgetArea) = 0;
	virtual void RemoveDockWidget(QWidget*) = 0;
};

class CORE_EXPORT IEditor : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() { return UUID; }
	virtual void GetCurrentLineIndex() = 0;
};

class CORE_EXPORT IFileManipulator : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() { return UUID; }
	virtual void Open(QStringList const&) = 0;
	virtual void CloseAll(int = 0) = 0;
	virtual void SaveAll() = 0;
	virtual void Close(int = -1) = 0;
	virtual void Save(QString const& = "") = 0;
	virtual void New() = 0;
};