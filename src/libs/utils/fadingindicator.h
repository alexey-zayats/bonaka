#ifndef FADINGINDICATOR_H
#define FADINGINDICATOR_H

#include "utils_global.h"

#include <QString>
#include <QWidget>

namespace Utils {
namespace FadingIndicator {

enum TextSize {
    SmallText,
    LargeText
};

UTILS_EXPORT void showText(QWidget *parent, const QString &text,
                                     TextSize size = LargeText);
UTILS_EXPORT void showPixmap(QWidget *parent, const QString &pixmap);

} // FadingIndicator
} // Utils

#endif // FADINGINDICATOR_H

