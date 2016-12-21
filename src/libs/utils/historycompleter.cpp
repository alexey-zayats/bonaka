#include "historycompleter.h"
#include "lineedit.h"
#include "theme/theme.h"
#include "utilsicons.h"

#include <QSettings>

#include <QItemDelegate>
#include <QKeyEvent>
#include <QListView>
#include <QPainter>

namespace Utils {
namespace Internal {

static QSettings *theSettings = 0;

class HistoryCompleterPrivate : public QAbstractListModel
{
public:
    HistoryCompleterPrivate() : maxLines(30) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    void clearHistory();
    void addEntry(const QString &str);

    QStringList list;
    QString historyKey;
    bool isLastItemEmpty;
    QString historyKeyIsLastItemEmpty;
    int maxLines;
};

class HistoryLineDelegate : public QItemDelegate
{
public:
    HistoryLineDelegate(QObject *parent)
        : QItemDelegate(parent)
        , pixmap(Icons::EDIT_CLEAR.pixmap())
    {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QItemDelegate::paint(painter,option,index);
        QRect r = QStyle::alignedRect(option.direction, Qt::AlignRight | Qt::AlignVCenter , pixmap.size(), option.rect);
        painter->drawPixmap(r, pixmap);
    }

    QPixmap pixmap;
};

class HistoryLineView : public QListView
{
public:
    HistoryLineView(HistoryCompleterPrivate *model_)
        : model(model_)
    {
        HistoryLineDelegate *delegate = new HistoryLineDelegate(this);
        pixmapWidth = delegate->pixmap.width();
        setItemDelegate(delegate);
    }

private:
    void mousePressEvent(QMouseEvent *event)
    {
        int rr= event->x();
        if (layoutDirection() == Qt::LeftToRight)
            rr = viewport()->width() - event->x();
        if (rr < pixmapWidth) {
            model->removeRow(indexAt(event->pos()).row());
            return;
        }
        QListView::mousePressEvent(event);
    }

    HistoryCompleterPrivate *model;
    int pixmapWidth;
};

} // namespace Internal

using namespace Internal;

int HistoryCompleterPrivate::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : list.count();
}

QVariant HistoryCompleterPrivate::data(const QModelIndex &index, int role) const
{
    if (index.row() >= list.count() || index.column() != 0)
        return QVariant();
    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return list.at(index.row());
    return QVariant();
}

bool HistoryCompleterPrivate::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row + count > list.count())
        return false;
    beginRemoveRows(parent, row, row + count -1);
    for (int i = 0; i < count; ++i)
        list.removeAt(row);
    theSettings->setValue(historyKey, list);
    endRemoveRows();
    return true;
}

void HistoryCompleterPrivate::clearHistory()
{
    beginResetModel();
    list.clear();
    endResetModel();
}

void HistoryCompleterPrivate::addEntry(const QString &str)
{
    const QString entry = str.trimmed();
    if (entry.isEmpty()) {
        isLastItemEmpty = true;
        theSettings->setValue(historyKeyIsLastItemEmpty, isLastItemEmpty);
        return;
    }
    int removeIndex = list.indexOf(entry);
    beginResetModel();
    if (removeIndex != -1)
        list.removeAt(removeIndex);
    list.prepend(entry);
    list = list.mid(0, maxLines - 1);
    endResetModel();
    theSettings->setValue(historyKey, list);
    isLastItemEmpty = false;
    theSettings->setValue(historyKeyIsLastItemEmpty, isLastItemEmpty);
}

HistoryCompleter::HistoryCompleter(const QString &historyKey, QObject *parent)
    : QCompleter(parent),
      d(new HistoryCompleterPrivate)
{
    d->historyKey = QLatin1String("CompleterHistory/") + historyKey;
    d->list = theSettings->value(d->historyKey).toStringList();
    d->historyKeyIsLastItemEmpty = QLatin1String("CompleterHistory/")
        + historyKey + QLatin1String(".IsLastItemEmpty");
    d->isLastItemEmpty = theSettings->value(d->historyKeyIsLastItemEmpty, false).toBool();

    setModel(d);
    setPopup(new HistoryLineView(d));
}

bool HistoryCompleter::removeHistoryItem(int index)
{
    return d->removeRow(index);
}

QString HistoryCompleter::historyItem() const
{
    if (historySize() == 0 || d->isLastItemEmpty)
        return QString();
    return d->list.at(0);
}

bool HistoryCompleter::historyExistsFor(const QString &historyKey)
{
    const QString fullKey = QLatin1String("CompleterHistory/") + historyKey;
    return theSettings->value(fullKey).isValid();
}

HistoryCompleter::~HistoryCompleter()
{
    delete d;
}

int HistoryCompleter::historySize() const
{
    return d->rowCount();
}

int HistoryCompleter::maximalHistorySize() const
{
    return d->maxLines;
}

void HistoryCompleter::setMaximalHistorySize(int numberOfEntries)
{
    d->maxLines = numberOfEntries;
}

void HistoryCompleter::clearHistory()
{
    d->clearHistory();
}

void HistoryCompleter::addEntry(const QString &str)
{
    d->addEntry(str);
}

void HistoryCompleter::setSettings(QSettings *settings)
{
    Internal::theSettings = settings;
}

} // namespace Utils
