#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(APPLICATION_LIB)
#  define APPLICATION_EXPORT Q_DECL_EXPORT
# else
#  define APPLICATION_EXPORT Q_DECL_IMPORT
# endif
#else
# define APPLICATION_EXPORT
#endif
