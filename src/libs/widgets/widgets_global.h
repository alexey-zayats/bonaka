#ifndef WIDGETS_GLOBAL_H
#define WIDGETS_GLOBAL_H

#include <QLoggingCategory>
#include <qglobal.h>

#if defined(WIDGETS_LIBRARY)
#  define WIDGETS_EXPORT Q_DECL_EXPORT
#else
#  define WIDGETS_EXPORT Q_DECL_IMPORT
#endif

Q_DECLARE_LOGGING_CATEGORY(widgetsLog)

#endif // WIDGETS_GLOBAL_H
