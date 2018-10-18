#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(CONSOLE_LIB)
#  define CONSOLE_EXPORT Q_DECL_EXPORT
# else
#  define CONSOLE_EXPORT Q_DECL_IMPORT
# endif
#else
# define CONSOLE_EXPORT
#endif
