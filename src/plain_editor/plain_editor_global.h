#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PLAIN_EDITOR_LIB)
#  define PLAIN_EDITOR_EXPORT Q_DECL_EXPORT
# else
#  define PLAIN_EDITOR_EXPORT Q_DECL_IMPORT
# endif
#else
# define PLAIN_EDITOR_EXPORT
#endif
