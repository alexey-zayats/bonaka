#ifndef CALCULATE_GLOBAL_H
#define CALCULATE_GLOBAL_H

#include <QLoggingCategory>
#include <qglobal.h>

#if defined(CALCULATE_LIBRARY)
#  define CALCULATE_EXPORT Q_DECL_EXPORT
#else
#  define CALCULATE_EXPORT Q_DECL_IMPORT
#endif

Q_DECLARE_LOGGING_CATEGORY(calculateLog)

#endif // CALCULATE_GLOBAL_H
