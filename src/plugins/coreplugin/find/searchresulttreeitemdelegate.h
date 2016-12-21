#ifndef SEARCHRESULTTREEITEMDELEGATE_H
#define SEARCHRESULTTREEITEMDELEGATE_H

#include <QItemDelegate>

namespace Core {
namespace Internal {

class SearchResultTreeItemDelegate: public QItemDelegate
{
public:
    SearchResultTreeItemDelegate(int tabWidth, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setTabWidth(int width);

private:
    int drawLineNumber(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, const QModelIndex &index) const;
    void drawText(QPainter *painter, const QStyleOptionViewItem &option,
                           const QRect &rect, const QModelIndex &index) const;

    QString m_tabString;
    static const int m_minimumLineNumberDigits = 6;
};

} // namespace Internal
} // namespace Core

#endif // SEARCHRESULTTREEITEMDELEGATE_H
