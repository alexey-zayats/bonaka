#include "categorysortfiltermodel.h"

namespace Utils {

CategorySortFilterModel::CategorySortFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool CategorySortFilterModel::filterAcceptsRow(int source_row,
                                               const QModelIndex &source_parent) const
{
    if (!source_parent.isValid()) {
        // category items should be visible if any of its children match
        const QRegExp &regexp = filterRegExp();
        const QModelIndex &categoryIndex = sourceModel()->index(source_row, 0, source_parent);
        if (regexp.indexIn(sourceModel()->data(categoryIndex, filterRole()).toString()) != -1)
            return true;
        const int rowCount = sourceModel()->rowCount(categoryIndex);
        const int columnCount = sourceModel()->columnCount(categoryIndex);
        for (int row = 0; row < rowCount; ++row) {
            for (int column = 0; column < columnCount; ++column) {
                if (regexp.indexIn(sourceModel()->data(
                                       sourceModel()->index(row, column, categoryIndex),
                                       filterRole()).toString()) != -1)
                    return true;
            }
        }
        return false;
    }
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

} // Utils

