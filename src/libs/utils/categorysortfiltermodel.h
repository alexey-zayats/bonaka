
#ifndef CATEGORYSORTFILTERMODEL_H
#define CATEGORYSORTFILTERMODEL_H

#include "utils_global.h"

#include <QSortFilterProxyModel>

namespace Utils {

class UTILS_EXPORT CategorySortFilterModel : public QSortFilterProxyModel
{
public:
    CategorySortFilterModel(QObject *parent = 0);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

} // Utils

#endif // CATEGORYSORTFILTERMODEL_H
