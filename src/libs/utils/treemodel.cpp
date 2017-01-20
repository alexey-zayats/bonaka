#include "treemodel.h"

#include <QStack>
#include <QSize>

#define USE_MODEL_TEST 0
#define USE_INDEX_CHECKS 0

#define CHECK_INDEX(index)

namespace Utils {

//
// TreeItem
//
TreeItem::TreeItem()
    : m_parent(0), m_model(0)
{
}

TreeItem::~TreeItem()
{
    removeChildren();
}

TreeItem *TreeItem::childAt(int pos) const
{
    return pos < m_children.size() ? m_children.at(pos) : 0;
}

QVariant TreeItem::data(int column, int role) const
{
    Q_UNUSED(column);
    Q_UNUSED(role);
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
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool TreeItem::hasChildren() const
{
    return canFetchMore() || childCount() > 0;
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
    if (childCount() == 0)
        return;
    if (m_model) {
        QModelIndex idx = index();
        m_model->beginRemoveRows(idx, 0, childCount() - 1);
        clear();
        m_model->endRemoveRows();
    } else {
        clear();
    }
}

void TreeItem::sortChildren(const std::function<bool(const TreeItem *, const TreeItem *)> &cmp)
{
    if (m_model) {
        if (const int n = childCount()) {
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

void TreeItem::updateAll()
{
    if (m_model) {
        QModelIndex idx = index();
        m_model->dataChanged(idx, idx.sibling(idx.row(), m_model->m_columnCount - 1));
        foreach (TreeItem *item, m_children)
            item->updateAll();
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

int TreeItem::indexInParent() const
{
    return m_parent ? m_parent->m_children.indexOf(const_cast<TreeItem *>(this)) : -1;
}

QModelIndex TreeItem::index() const
{
    return m_model->indexForItem(this);
}

QAbstractItemModel *TreeItem::model() const
{
    return m_model;
}

void TreeItem::forAllChildren(const std::function<void (TreeItem *)> &pred) const
{
    foreach (TreeItem *item, m_children) {
        pred(item);
        item->forAllChildren(pred);
    }
}

void TreeItem::forSelectedChildren(const std::function<bool (TreeItem *)> &pred) const
{
    foreach (TreeItem *item, m_children) {
        if (pred(item))
            item->forSelectedChildren(pred);
    }
}

void TreeItem::forChildrenAtLevel(int level, const std::function<void(TreeItem *)> &pred) const
{
    if (level == 1) {
        foreach (TreeItem *item, m_children)
            pred(item);
    } else {
        foreach (TreeItem *item, m_children)
            item->forChildrenAtLevel(level - 1, pred);
    }
}

TreeItem *TreeItem::findChildAtLevel(int level, const std::function<bool(TreeItem *)> &pred) const
{
    if (level == 1) {
        foreach (TreeItem *item, m_children)
            if (pred(item))
                return item;
    } else {
        foreach (TreeItem *item, m_children) {
            if (auto found = item->findChildAtLevel(level - 1, pred))
                return found;
        }
    }
    return 0;
}

TreeItem *TreeItem::findAnyChild(const std::function<bool(TreeItem *)> &pred) const
{
    foreach (TreeItem *item, m_children) {
        if (pred(item))
            return item;
        if (TreeItem *found = item->findAnyChild(pred))
            return found;
    }
    return 0;
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

void TreeItem::propagateModel(BaseTreeModel *m)
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

BaseTreeModel::BaseTreeModel(QObject *parent)
    : QAbstractItemModel(parent),
      m_root(new TreeItem)
{
    m_columnCount = 1;
    m_root->m_model = this;
}

BaseTreeModel::BaseTreeModel(TreeItem *root, QObject *parent)
    : QAbstractItemModel(parent),
      m_root(root)
{
    m_columnCount = 1;
    m_root->propagateModel(this);
}

BaseTreeModel::~BaseTreeModel()
{
    m_root->m_model = 0;
    delete m_root;
}

QModelIndex BaseTreeModel::parent(const QModelIndex &idx) const
{
    CHECK_INDEX(idx);
    if (!idx.isValid())
        return QModelIndex();

    const TreeItem *item = itemForIndex(idx);
    const TreeItem *parent = item->parent();
    if (!parent || parent == m_root)
        return QModelIndex();

    const TreeItem *grandparent = parent->parent();
    if (!grandparent)
        return QModelIndex();

    for (int i = 0, n = grandparent->childCount(); i < n; ++i)
        if (grandparent->childAt(i) == parent)
            return createIndex(i, 0, (void*) parent);

    return QModelIndex();
}

int BaseTreeModel::rowCount(const QModelIndex &idx) const
{
    CHECK_INDEX(idx);
    if (!idx.isValid())
        return m_root->childCount();
    if (idx.column() > 0)
        return 0;
    const TreeItem *item = itemForIndex(idx);
    return item->childCount();
}

int BaseTreeModel::columnCount(const QModelIndex &idx) const
{
    CHECK_INDEX(idx);
    if (idx.column() > 0)
        return 0;
    return m_columnCount;
}

bool BaseTreeModel::setData(const QModelIndex &idx, const QVariant &data, int role)
{
    TreeItem *item = itemForIndex(idx);
    bool res = item ? item->setData(idx.column(), data, role) : false;
    if (res)
        emit dataChanged(idx, idx);
    return res;
}

QVariant BaseTreeModel::data(const QModelIndex &idx, int role) const
{
    TreeItem *item = itemForIndex(idx);
    return item ? item->data(idx.column(), role) : QVariant();
}

QVariant BaseTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < m_header.size())
        return m_header.at(section);
    if (role == Qt::ToolTipRole && section < m_headerToolTip.size())
        return m_headerToolTip.at(section);
    return QVariant();
}

bool BaseTreeModel::hasChildren(const QModelIndex &idx) const
{
    TreeItem *item = itemForIndex(idx);
    return !item || item->hasChildren();
}

Qt::ItemFlags BaseTreeModel::flags(const QModelIndex &idx) const
{
    if (!idx.isValid())
        return 0;
    TreeItem *item = itemForIndex(idx);
    return item ? item->flags(idx.column())
                : (Qt::ItemIsEnabled|Qt::ItemIsSelectable);
}

bool BaseTreeModel::canFetchMore(const QModelIndex &idx) const
{
    if (!idx.isValid())
        return false;
    TreeItem *item = itemForIndex(idx);
    return item ? item->canFetchMore() : false;
}

void BaseTreeModel::fetchMore(const QModelIndex &idx)
{
    if (!idx.isValid())
        return;
    TreeItem *item = itemForIndex(idx);
    if (item)
        item->fetchMore();
}

TreeItem *BaseTreeModel::rootItem() const
{
    return m_root;
}

void BaseTreeModel::setRootItem(TreeItem *item)
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

void BaseTreeModel::setHeader(const QStringList &displays)
{
    m_header = displays;
    m_columnCount = displays.size();
}

void BaseTreeModel::setHeaderToolTip(const QStringList &tips)
{
    m_headerToolTip = tips;
}

QModelIndex BaseTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    CHECK_INDEX(parent);
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    const TreeItem *item = itemForIndex(parent);
    if (row >= item->childCount())
        return QModelIndex();
    return createIndex(row, column, (void*)(item->childAt(row)));
}

TreeItem *BaseTreeModel::itemForIndex(const QModelIndex &idx) const
{
    CHECK_INDEX(idx);
    TreeItem *item = idx.isValid() ? static_cast<TreeItem*>(idx.internalPointer()) : m_root;
    return item;
}

QModelIndex BaseTreeModel::indexForItem(const TreeItem *item) const
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
void BaseTreeModel::clear()
{
    if (m_root)
        m_root->removeChildren();
}

/*!
   Removes the specified item from the model.

   \note The item is not destroyed, ownership is effectively passed to the caller.
   */

TreeItem *BaseTreeModel::takeItem(TreeItem *item)
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

void BaseTreeModel::destroyItem(TreeItem *item)
{
    delete takeItem(item);
}

StaticTreeItem::StaticTreeItem(const QStringList &displays)
    : m_displays(displays)
{
}

StaticTreeItem::StaticTreeItem(const QString &display)
    : m_displays(display)
{
}

QVariant StaticTreeItem::data(int column, int role) const
{
    if (role == Qt::DisplayRole && column >= 0 && column < m_displays.size())
        return m_displays.at(column);
    return QVariant();
}

Qt::ItemFlags StaticTreeItem::flags(int column) const
{
    Q_UNUSED(column);
    return Qt::ItemIsEnabled;
}

} // namespace Utils
