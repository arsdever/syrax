#pragma once

#include "core.h"
#include <QString>
#include <macros.h>

class CORE_EXPORT IUnknown
{
public:
	virtual QString const& GetUUID() const = 0;
	virtual ~IUnknown() = 0;
};

template <typename INTERFACE_TYPE, typename RETURN_TYPE = void>
struct CORE_EXPORT IFunctor
{
	virtual RETURN_TYPE operator() (INTERFACE_TYPE* plugin) = 0;
};

class CORE_EXPORT ILogger : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() const override { return UUID; }

	GENERATE_FUNCTOR_1(ILogger, Info, QString const&)
	GENERATE_FUNCTOR_1(ILogger, Warning, QString const&)
	GENERATE_FUNCTOR_1(ILogger, Error, QString const&)

protected:
	virtual void Info(QString const&) = 0;
	virtual void Warning(QString const&) = 0;
	virtual void Error(QString const&) = 0;
};

class CORE_EXPORT IApplication : public IUnknown
{
public:
	static const QString UUID;
	virtual QString const& GetUUID() const override { return UUID; }

	GENERATE_FUNCTOR_0(IApplication, Close)
	GENERATE_FUNCTOR_3_1(IApplication, AddDockWidget, QWidget*, QString const&, Qt::DockWidgetArea, Qt::NoDockWidgetArea)
	GENERATE_FUNCTOR_1(IApplication, RemoveDockWidget, QWidget*)

protected:
	virtual void Close() = 0;
	virtual void AddDockWidget(QWidget*, QString const&, Qt::DockWidgetArea) = 0;
	virtual void RemoveDockWidget(QWidget*) = 0;
};

class CORE_EXPORT IFileManipulator : public IUnknown
{
public:
	enum EClosingType
	{
		Single,
		Right,
		Left,
		All
	};

	static const QString UUID;
	virtual QString const& GetUUID() const override { return UUID; }

	GENERATE_FUNCTOR_0(IFileManipulator, New)
	GENERATE_FUNCTOR_1(IFileManipulator, Open, QStringList const&)
	GENERATE_FUNCTOR_1_1(IFileManipulator, Save, QString const&, "")
	GENERATE_FUNCTOR_0(IFileManipulator, SaveAs)
	GENERATE_FUNCTOR_0(IFileManipulator, SaveAll)
	GENERATE_FUNCTOR_1_1(IFileManipulator, Close, qint32, -1)
	RET_GENERATE_FUNCTOR_2_2(bool, IFileManipulator, AskForClose, qint32, EClosingType, -1, Single)

protected:
	virtual void New		() = 0;
	virtual void Open		(QStringList const&) = 0;
	virtual void Save		(QString const&) = 0;
	virtual void SaveAs		() = 0;
	virtual void SaveAll	() = 0;
	virtual void Close		(qint32) = 0;
	virtual bool AskForClose(qint32, EClosingType) = 0;
};