#ifndef ESTIMATION_GLOBAL_H
#define ESTIMATION_GLOBAL_H

#include <QLoggingCategory>
#include <qglobal.h>

#if defined(ESTIMATION_LIBRARY)
#  define ESTIMATION_EXPORT Q_DECL_EXPORT
#else
#  define ESTIMATION_EXPORT Q_DECL_IMPORT
#endif

Q_DECLARE_LOGGING_CATEGORY(estimationLog)

#endif // ESTIMATION_GLOBAL_H
