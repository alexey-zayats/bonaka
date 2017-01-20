#ifndef REGISTRY_GLOBAL_H
#define REGISTRY_GLOBAL_H

#include <QLoggingCategory>
#include <qglobal.h>

#if defined(REGISTRY_LIBRARY)
#  define REGISTRY_EXPORT Q_DECL_EXPORT
#else
#  define REGISTRY_EXPORT Q_DECL_IMPORT
#endif

Q_DECLARE_LOGGING_CATEGORY(registryLog)

#endif // REGISTRY_GLOBAL_H
