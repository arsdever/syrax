#pragma once

#include <core.h>
#include <interfaces.h>

template <typename _inttype_ >
struct CORE_EXPORT Range
{
	_inttype_ from;
	_inttype_ to;
};

int CORE_EXPORT DigitCount(int number);

template <typename INTERFACE_TYPE>
void CallFunction(IFunctor<INTERFACE_TYPE>& functor)
{
	QSet<INTERFACE_TYPE*> plugin_list = GetCore()->QueryInterface<INTERFACE_TYPE>();
	for (INTERFACE_TYPE* plugin : plugin_list)
	{
		if (plugin == nullptr)
			continue;

		functor(plugin);
	}
}
template <typename INTERFACE_TYPE, typename RETURN_TYPE>
QMap<void*, RETURN_TYPE> CallFunctionAndReturn(IFunctor<INTERFACE_TYPE, RETURN_TYPE>& functor)
{
	QMap<void*, RETURN_TYPE> result;
	QSet<INTERFACE_TYPE*> plugin_list = GetCore()->QueryInterface<INTERFACE_TYPE>();
	for (INTERFACE_TYPE* plugin : plugin_list)
	{
		if (plugin == nullptr)
			continue;

		result[plugin] = functor(plugin);
	}
	return result;
}