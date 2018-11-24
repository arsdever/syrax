#pragma once

template <typename T>
QSet<T*> CCore::QueryInterface()
{
	if (!s_mapPlugins.contains(T::UUID))
		return QSet<T*>();

	QSet<T*> result;
	for (IUnknown* plg : s_mapPlugins[T::UUID])
	{
		T* plgPtr = dynamic_cast<T*> (plg);
		if (plgPtr == nullptr)
			continue;

		result.insert(plgPtr);
	}
	return result;
}