#include "ifindsupport.h"

#include <utils/fadingindicator.h>
#include <utils/stylehelper.h>

using namespace Core;

void IFindSupport::replace(const QString &before, const QString &after, FindFlags findFlags)
{
    Q_UNUSED(before)
    Q_UNUSED(after)
    Q_UNUSED(findFlags)
}

bool IFindSupport::replaceStep(const QString &before, const QString &after, FindFlags findFlags)
{
    Q_UNUSED(before)
    Q_UNUSED(after)
    Q_UNUSED(findFlags)
    return false;
}

int IFindSupport::replaceAll(const QString &before, const QString &after, FindFlags findFlags)
{
    Q_UNUSED(before)
    Q_UNUSED(after)
    Q_UNUSED(findFlags)
    return 0;
}

void IFindSupport::showWrapIndicator(QWidget *parent)
{
    Utils::FadingIndicator::showPixmap(parent, Utils::StyleHelper::dpiSpecificImageFile(
                                           QLatin1String(":/find/images/wrapindicator.png")));
}
