#pragma once

#include <QtCore\qglobal.h>

#ifndef BUILD_STATIC
# if defined(PLUGIN_LIB)
#  define PLUGIN_EXPORT Q_DECL_EXPORT
# else
#  define PLUGIN_EXPORT Q_DECL_IMPORT
# endif
#else
# define PLUGIN_EXPORT
#endif
