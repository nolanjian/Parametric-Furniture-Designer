#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(SCENE_LIB)
#  define SCENE_EXPORT Q_DECL_EXPORT
# else
#  define SCENE_EXPORT Q_DECL_IMPORT
# endif
#else
# define SCENE_EXPORT
#endif
