#ifndef HEADERVIEWSTRETCHER_H
#define HEADERVIEWSTRETCHER_H

#include "utils_global.h"
#include <QObject>

QT_BEGIN_NAMESPACE
class QHeaderView;
QT_END_NAMESPACE

namespace Utils {

class UTILS_EXPORT HeaderViewStretcher : public QObject
{
    const int m_columnToStretch;
public:
    explicit HeaderViewStretcher(QHeaderView *headerView, int columnToStretch);

    void stretch();
    bool eventFilter(QObject *obj, QEvent *ev) override;
};

} // namespace Utils

#endif // HEADERVIEWSTRETCHER_H
