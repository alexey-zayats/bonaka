#ifndef SEARCHRESULTTREEVIEW_H
#define SEARCHRESULTTREEVIEW_H

#include "searchresultwindow.h"

#include <utils/itemviews.h>

namespace Core {
namespace Internal {

class SearchResultTreeModel;
class SearchResultColor;

class SearchResultTreeView : public Utils::TreeView
{
    Q_OBJECT

public:
    explicit SearchResultTreeView(QWidget *parent = 0);

    void setAutoExpandResults(bool expand);
    void setTextEditorFont(const QFont &font, const SearchResultColor &color);
    void setTabWidth(int tabWidth);

    SearchResultTreeModel *model() const;
    void addResults(const QList<SearchResultItem> &items, SearchResult::AddMode mode);

signals:
    void jumpToSearchResult(const SearchResultItem &item);

public slots:
    void clear();
    void emitJumpToSearchResult(const QModelIndex &index);

protected:
    SearchResultTreeModel *m_model;
    bool m_autoExpandResults;
};

} // namespace Internal
} // namespace Core

#endif // SEARCHRESULTTREEVIEW_H
