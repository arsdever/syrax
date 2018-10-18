#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(BREAKPOINTS_LIB)
#  define BREAKPOINTS_EXPORT Q_DECL_EXPORT
# else
#  define BREAKPOINTS_EXPORT Q_DECL_IMPORT
# endif
#else
# define BREAKPOINTS_EXPORT
#endif
