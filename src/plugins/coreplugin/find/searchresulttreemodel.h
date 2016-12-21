#ifndef SEARCHRESULTTREEMODEL_H
#define SEARCHRESULTTREEMODEL_H

#include "searchresultwindow.h"
#include "searchresultcolor.h"

#include <QAbstractItemModel>
#include <QFont>

namespace Core {
namespace Internal {

class SearchResultTreeItem;

class SearchResultTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    SearchResultTreeModel(QObject *parent = 0);
    ~SearchResultTreeModel();

    void setShowReplaceUI(bool show);
    void setTextEditorFont(const QFont &font, const SearchResultColor &color);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QModelIndex next(const QModelIndex &idx, bool includeGenerated = false, bool *wrapped = 0) const;
    QModelIndex prev(const QModelIndex &idx, bool includeGenerated = false, bool *wrapped = 0) const;

    QList<QModelIndex> addResults(const QList<SearchResultItem> &items, SearchResult::AddMode mode);

signals:
    void jumpToSearchResult(const QString &fileName, int lineNumber,
                            int searchTermStart, int searchTermLength);

public slots:
    void clear();

private:
    QModelIndex index(SearchResultTreeItem *item) const;
    void addResultsToCurrentParent(const QList<SearchResultItem> &items, SearchResult::AddMode mode);
    QSet<SearchResultTreeItem *> addPath(const QStringList &path);
    QVariant data(const SearchResultTreeItem *row, int role) const;
    bool setCheckState(const QModelIndex &idx, Qt::CheckState checkState, bool firstCall = true);
    QModelIndex nextIndex(const QModelIndex &idx, bool *wrapped = 0) const;
    QModelIndex prevIndex(const QModelIndex &idx, bool *wrapped = 0) const;
    SearchResultTreeItem *treeItemAtIndex(const QModelIndex &idx) const;

    SearchResultTreeItem *m_rootItem;
    SearchResultTreeItem *m_currentParent;
    SearchResultColor m_color;
    QModelIndex m_currentIndex;
    QStringList m_currentPath; // the path that belongs to the current parent
    QFont m_textEditorFont;
    bool m_showReplaceUI;
    bool m_editorFontIsUsed;
};

} // namespace Internal
} // namespace Core

#endif // SEARCHRESULTTREEMODEL_H
