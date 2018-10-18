#pragma once

#include <core.h>

template <typename _inttype_ >
struct CORE_EXPORT Range
{
	_inttype_ from;
	_inttype_ to;
};

int CORE_EXPORT DigitCount(int number);