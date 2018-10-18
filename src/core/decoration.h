#pragma once

template <typename WIDGET_BASE>
class IDecoration : public WIDGET_BASE
{
public:
	IDecoration(WIDGET_BASE* pCore)
		: m_pCore(pCore)
	{}

	virtual ~IDecoration() {}

	WIDGET_BASE* CoreWidget() const { return m_pCore; }

private:
	WIDGET_BASE* m_pCore;
};