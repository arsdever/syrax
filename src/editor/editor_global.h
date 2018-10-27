#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(EDITOR_LIB)
#  define EDITOR_EXPORT Q_DECL_EXPORT
# else
#  define EDITOR_EXPORT Q_DECL_IMPORT
# endif
#else
# define EDITOR_EXPORT
#endif
