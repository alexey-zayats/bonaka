#ifndef SEARCHRESULTTREEITEMS_H
#define SEARCHRESULTTREEITEMS_H

#include "searchresultwindow.h"

#include <QString>
#include <QList>

namespace Core {
namespace Internal {

class SearchResultTreeItem
{
public:
    explicit SearchResultTreeItem(const SearchResultItem &item = SearchResultItem(),
                                  SearchResultTreeItem *parent = NULL);
    virtual ~SearchResultTreeItem();

    bool isLeaf() const;
    SearchResultTreeItem *parent() const;
    SearchResultTreeItem *childAt(int index) const;
    int insertionIndex(const QString &text, SearchResultTreeItem **existingItem) const;
    int insertionIndex(const SearchResultItem &item, SearchResultTreeItem **existingItem) const;
    void insertChild(int index, SearchResultTreeItem *child);
    void insertChild(int index, const SearchResultItem &item);
    void appendChild(const SearchResultItem &item);
    int childrenCount() const;
    int rowOfItem() const;
    void clearChildren();

    Qt::CheckState checkState() const;
    void setCheckState(Qt::CheckState checkState);

    bool isGenerated() const { return m_isGenerated; }
    void setGenerated(bool value) { m_isGenerated = value; }

    SearchResultItem item;

private:
    SearchResultTreeItem *m_parent;
    QList<SearchResultTreeItem *> m_children;
    bool m_isGenerated;
    Qt::CheckState m_checkState;
};

} // namespace Internal
} // namespace Core

#endif // SEARCHRESULTTREEITEMS_H
