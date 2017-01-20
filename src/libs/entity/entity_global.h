#ifndef ENTITY_GLOBAL_H
#define ENTITY_GLOBAL_H

#include <QLoggingCategory>
#include <qglobal.h>

#if defined(APP_ENTITY_LIB)
#  define ENTITY_EXPORT Q_DECL_EXPORT
#else
#  define ENTITY_EXPORT Q_DECL_IMPORT
#endif

Q_DECLARE_LOGGING_CATEGORY(entityLog)

#endif // ENTITY_GLOBAL_H
