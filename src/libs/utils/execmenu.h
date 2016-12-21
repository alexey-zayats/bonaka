
#ifndef EXECMENU_H
#define EXECMENU_H

#include "utils_global.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QWidget;
QT_END_NAMESPACE

namespace Utils {

UTILS_EXPORT QAction *execMenuAtWidget(QMenu *menu, QWidget *widget);

} // namespace Utils

#endif // EXECMENU_H
