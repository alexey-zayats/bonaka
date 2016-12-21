#ifndef UTILS_GLOBAL_H
#define UTILS_GLOBAL_H

#include <QLoggingCategory>
#include <qglobal.h>

#if defined(APP_UTILS_LIB)
#  define UTILS_EXPORT Q_DECL_EXPORT
#else
#  define UTILS_EXPORT Q_DECL_IMPORT
#endif

Q_DECLARE_LOGGING_CATEGORY(utilsLog)

#endif // UTILS_GLOBAL_H
