#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(CORE_LIB)
#  define CORE_EXPORT Q_DECL_EXPORT
# else
#  define CORE_EXPORT Q_DECL_IMPORT
# endif
#else
# define CORE_EXPORT
#endif