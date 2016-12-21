
#ifndef CORE_GLOBAL_H
#define CORE_GLOBAL_H

#include <QLoggingCategory>
#include <qglobal.h>

#if defined(CORE_LIBRARY)
#  define CORE_EXPORT Q_DECL_EXPORT
#else
#  define CORE_EXPORT Q_DECL_IMPORT
#endif

Q_DECLARE_LOGGING_CATEGORY(corepluginLog)

#endif // CORE_GLOBAL_H
