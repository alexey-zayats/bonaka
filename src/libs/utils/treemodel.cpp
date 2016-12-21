#include "treemodel.h"

#include <QStack>
#include <QSize>

namespace Utils {

//
// TreeItem
//
TreeItem::TreeItem()
    : m_parent(0), m_model(0), m_displays(0), m_flags(Qt::ItemIsEnabled|Qt::ItemIsSelectable)
{
}

TreeItem::TreeItem(const QStringList &displays, int flags)
    : m_parent(0), m_model(0), m_displays(new QStringList(displays)), m_flags(flags)
{
}

TreeItem::~TreeItem()
{
    removeChildren();
    delete m_displays;
}

TreeItem *TreeItem::child(int pos) const
{
    return pos < m_children.size() ? m_children.at(pos) : 0;
}

int TreeItem::rowCount() const
{
    return m_children.size();
}

QVariant TreeItem::data(int column, int role) const
{
    if (role == Qt::DisplayRole && m_displays && column >= 0 && column < m_displays->size())
        return m_displays->at(column);
    return QVariant();
}

bool TreeItem::setData(int column, const QVariant &data, int role)
{
    Q_UNUSED(column);
    Q_UNUSED(data);
    Q_UNUSED(role);
    return false;
}

Qt::ItemFlags TreeItem::flags(int column) const
{
    Q_UNUSED(column);
    return m_flags;
}

bool TreeItem::hasChildren() const
{
    return canFetchMore() || rowCount() > 0;
}

bool TreeItem::canFetchMore() const
{
    return false;
}

void TreeItem::prependChild(TreeItem *item)
{
    insertChild(0, item);
}

void TreeItem::appendChild(TreeItem *item)
{
    insertChild(m_children.size(), item);
}

void TreeItem::insertChild(int pos, TreeItem *item)
{
    if (m_model) {
        QModelIndex idx = index();
        m_model->beginInsertRows(idx, pos, pos);
        item->m_parent = this;
        item->propagateModel(m_model);
        m_children.insert(m_children.begin() + pos, item);
        m_model->endInsertRows();
    } else {
        item->m_parent = this;
        if (m_model)
            item->propagateModel(m_model);
        m_children.insert(m_children.begin() + pos, item);
    }
}

void TreeItem::removeChildren()
{
    if (rowCount() == 0)
        return;
    if (m_model) {
        QModelIndex idx = index();
        m_model->beginRemoveRows(idx, 0, rowCount() - 1);
        clear();
        m_model->endRemoveRows();
    } else {
        clear();
    }
}

void TreeItem::sortChildren(const std::function<bool(const TreeItem *, const TreeItem *)> &cmp)
{
    if (m_model) {
        if (const int n = rowCount()) {
            QVector<TreeItem *> tmp = m_children;
            std::sort(tmp.begin(), tmp.end(), cmp);
            if (tmp == m_children) {
                // Nothing changed.
            } else {
                QModelIndex idx = index();
                m_model->beginRemoveRows(idx, 0, n - 1);
                m_children.clear();
                m_model->endRemoveRows();
                m_model->beginInsertRows(idx, 0, n - 1);
                tmp.swap(m_children);
                m_model->endInsertRows();
            }
        }
    } else {
        std::sort(m_children.begin(), m_children.end(), cmp);
    }
}

void TreeItem::update()
{
    if (m_model) {
        QModelIndex idx = index();
        m_model->dataChanged(idx.sibling(idx.row(), 0), idx.sibling(idx.row(), m_model->m_columnCount - 1));
    }
}

void TreeItem::updateColumn(int column)
{
    if (m_model) {
        QModelIndex idx = index();
        m_model->dataChanged(idx.sibling(idx.row(), column), idx.sibling(idx.row(), column));
    }
}

TreeItem *TreeItem::firstChild() const
{
    return m_children.isEmpty() ? 0 : m_children.first();
}

TreeItem *TreeItem::lastChild() const
{
    return m_children.isEmpty() ? 0 : m_children.last();
}

int TreeItem::level() const
{
    int l = 0;
    for (TreeItem *item = this->parent(); item; item = item->parent())
        ++l;
    return l;
}

void TreeItem::setFlags(Qt::ItemFlags flags)
{
    m_flags = flags;
}

QModelIndex TreeItem::index() const
{
    return m_model->indexForItem(this);
}

void TreeItem::walkTree(TreeItemVisitor *visitor)
{
    if (visitor->preVisit(this)) {
        ++visitor->m_level;
        visitor->visit(this);
        foreach (TreeItem *item, m_children)
            item->walkTree(visitor);
        --visitor->m_level;
    }
    visitor->postVisit(this);
}

void TreeItem::walkTree(std::function<void (TreeItem *)> f)
{
    f(this);
    foreach (TreeItem *item, m_children)
        item->walkTree(f);
}

void TreeItem::clear()
{
    while (m_children.size()) {
        TreeItem *item = m_children.takeLast();
        item->m_model = 0;
        item->m_parent = 0;
        delete item;
    }
}

void TreeItem::expand()
{
    m_model->requestExpansion(index());
}

void TreeItem::propagateModel(TreeModel *m)
{
    if (m && !m_model) {
        m_model = m;
        foreach (TreeItem *item, m_children)
            item->propagateModel(m);
    }
}

/*!
    \class Utils::TreeModel

    \brief The TreeModel class is a convienience base class for models
    to use in a QTreeView.
*/

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent),
      m_root(new TreeItem)
{
    m_columnCount = 1;
    m_root->m_model = this;
#if USE_MODEL_TEST
    new ModelTest(this, this);
#endif
}

TreeModel::TreeModel(TreeItem *root, QObject *parent)
    : QAbstractItemModel(parent),
      m_root(root)
{
    m_columnCount = 1;
    m_root->propagateModel(this);
}

TreeModel::~TreeModel()
{
    m_root->m_model = 0;
    delete m_root;
}

QModelIndex TreeModel::parent(const QModelIndex &idx) const
{
    if (!idx.isValid())
        return QModelIndex();

    const TreeItem *item = itemForIndex(idx);
    const TreeItem *parent = item->parent();
    if (!parent || parent == m_root)
        return QModelIndex();

    const TreeItem *grandparent = parent->parent();
    if (!grandparent)
        return QModelIndex();

    for (int i = 0, n = grandparent->rowCount(); i < n; ++i)
        if (grandparent->child(i) == parent)
            return createIndex(i, 0, (void*) parent);

    return QModelIndex();
}

int TreeModel::rowCount(const QModelIndex &idx) const
{
    if (!idx.isValid())
        return m_root->rowCount();
    if (idx.column() > 0)
        return 0;
    const TreeItem *item = itemForIndex(idx);
    return item->rowCount();
}

int TreeModel::columnCount(const QModelIndex &idx) const
{
    if (idx.column() > 0)
        return 0;
    return m_columnCount;
}

bool TreeModel::setData(const QModelIndex &idx, const QVariant &data, int role)
{
    TreeItem *item = itemForIndex(idx);
    bool res = item ? item->setData(idx.column(), data, role) : false;
    if (res)
        emit dataChanged(idx, idx);
    return res;
}

QVariant TreeModel::data(const QModelIndex &idx, int role) const
{
    TreeItem *item = itemForIndex(idx);
    return item ? item->data(idx.column(), role) : QVariant();
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < m_header.size())
        return m_header.at(section);
    if (role == Qt::ToolTipRole && section < m_headerToolTip.size())
        return m_headerToolTip.at(section);
    return QVariant();
}

bool TreeModel::hasChildren(const QModelIndex &idx) const
{
    TreeItem *item = itemForIndex(idx);
    return !item || item->hasChildren();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &idx) const
{
    if (!idx.isValid())
        return 0;
    TreeItem *item = itemForIndex(idx);
    return item ? item->flags(idx.column())
                : (Qt::ItemIsEnabled|Qt::ItemIsSelectable);
}

bool TreeModel::canFetchMore(const QModelIndex &idx) const
{
    if (!idx.isValid())
        return false;
    TreeItem *item = itemForIndex(idx);
    return item ? item->canFetchMore() : false;
}

void TreeModel::fetchMore(const QModelIndex &idx)
{
    if (!idx.isValid())
        return;
    TreeItem *item = itemForIndex(idx);
    if (item)
        item->fetchMore();
}

TreeItem *TreeModel::rootItem() const
{
    return m_root;
}

int TreeModel::topLevelItemCount() const
{
    return m_root->childCount();
}

void TreeModel::setRootItem(TreeItem *item)
{
    emit layoutAboutToBeChanged();
    if (m_root) {
        // needs to be done explicitly before setting the model to 0, otherwise it might lead to a
        // crash inside a view or proxy model, especially if there are selected items
        m_root->removeChildren();
        m_root->m_model = 0;
        delete m_root;
    }
    m_root = item;
    item->propagateModel(this);
    emit layoutChanged();
}

void TreeModel::setHeader(const QStringList &displays)
{
    m_header = displays;
    m_columnCount = displays.size();
}

void TreeModel::setHeaderToolTip(const QStringList &tips)
{
    m_headerToolTip = tips;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    const TreeItem *item = itemForIndex(parent);
    if (row >= item->rowCount())
        return QModelIndex();
    return createIndex(row, column, (void*)(item->child(row)));
}

TreeItem *TreeModel::itemForIndex(const QModelIndex &idx) const
{
    TreeItem *item = idx.isValid() ? static_cast<TreeItem*>(idx.internalPointer()) : m_root;
    return item;
}

QModelIndex TreeModel::indexForItem(const TreeItem *item) const
{
    if (item == m_root)
        return QModelIndex();

    TreeItem *p = item->parent();

    TreeItem *mitem = const_cast<TreeItem *>(item);
    int row = p->m_children.indexOf(mitem);
    return createIndex(row, 0, mitem);
}

/*!
  Destroys all items in them model except the invisible root item.
*/
void TreeModel::clear()
{
    if (m_root)
        m_root->removeChildren();
}

/*!
   Removes the specified item from the model.

   \note The item is not destroyed, ownership is effectively passed to the caller.
   */

TreeItem *TreeModel::takeItem(TreeItem *item)
{
#if USE_MODEL_TEST
    (void) new ModelTest(this, this);
#endif

    TreeItem *parent = item->parent();
    int pos = parent->m_children.indexOf(item);

    QModelIndex idx = indexForItem(parent);
    beginRemoveRows(idx, pos, pos);
    item->m_parent = 0;
    item->m_model = 0;
    parent->m_children.removeAt(pos);
    endRemoveRows();
    return item;
}

//
// TreeLevelItems
//

UntypedTreeLevelItems::UntypedTreeLevelItems(TreeItem *item, int level)
    : m_item(item), m_level(level)
{}

UntypedTreeLevelItems::const_iterator::const_iterator(TreeItem *base, int level)
    : m_level(level)
{
    if (base) {
        // "begin()"
        m_depth = 0;
        // Level x: The item m_item[x] is the m_pos[x]'th child of its
        // parent, out of m_size[x].
        m_pos[0] = 0;
        m_size[0] = 1;
        m_item[0] = base;
        goDown();
    } else {
        // "end()"
        m_depth = -1;
    }
}

UntypedTreeLevelItems::const_iterator UntypedTreeLevelItems::begin() const
{
    return const_iterator(m_item, m_level);
}

UntypedTreeLevelItems::const_iterator UntypedTreeLevelItems::end() const
{
    return const_iterator(0, m_level);
}

} // namespace Utils
