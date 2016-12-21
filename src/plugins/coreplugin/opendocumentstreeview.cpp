#include "opendocumentstreeview.h"

#include "coreicons.h"

#include <QApplication>
#include <QHeaderView>
#include <QPainter>
#include <QStyledItemDelegate>

namespace Core {
namespace Internal {

class OpenDocumentsDelegate : public QStyledItemDelegate
{
public:
    explicit OpenDocumentsDelegate(QObject *parent = 0);

    void setCloseButtonVisible(bool visible);
    void handlePressed(const QModelIndex &index);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    mutable QModelIndex pressedIndex;
    bool closeButtonVisible;
};

OpenDocumentsDelegate::OpenDocumentsDelegate(QObject *parent)
 : QStyledItemDelegate(parent),
   closeButtonVisible(true)
{
}

void OpenDocumentsDelegate::setCloseButtonVisible(bool visible)
{
    closeButtonVisible = visible;
}

void OpenDocumentsDelegate::handlePressed(const QModelIndex &index)
{
    if (index.column() == 1)
        pressedIndex = index;
}

void OpenDocumentsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
           const QModelIndex &index) const
{
    if (option.state & QStyle::State_MouseOver) {
        if ((QApplication::mouseButtons() & Qt::LeftButton) == 0)
            pressedIndex = QModelIndex();
        QBrush brush = option.palette.alternateBase();
        if (index == pressedIndex)
            brush = option.palette.dark();
        painter->fillRect(option.rect, brush);
    }


    QStyledItemDelegate::paint(painter, option, index);

    if (closeButtonVisible && index.column() == 1 && option.state & QStyle::State_MouseOver) {
        const QIcon icon = (option.state & QStyle::State_Selected) ?
                    Icons::CLOSE_BACKGROUND.icon()
                  : Icons::CLOSE_FOREGROUND.icon();

        QRect iconRect(option.rect.right() - option.rect.height(),
                       option.rect.top(),
                       option.rect.height(),
                       option.rect.height());

        icon.paint(painter, iconRect, Qt::AlignRight | Qt::AlignVCenter);
    }

}

} // namespace Internal

OpenDocumentsTreeView::OpenDocumentsTreeView(QWidget *parent) :
    Utils::TreeView(parent)
{
    m_delegate = new Internal::OpenDocumentsDelegate(this);
    setItemDelegate(m_delegate);
    setIndentation(0);
    setUniformRowHeights(true);
    setTextElideMode(Qt::ElideMiddle);
    setFrameStyle(QFrame::NoFrame);
    setAttribute(Qt::WA_MacShowFocusRect, false);
    viewport()->setAttribute(Qt::WA_Hover);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setActivationMode(Utils::SingleClickActivation);

    installEventFilter(this);
    viewport()->installEventFilter(this);

    connect(this, &OpenDocumentsTreeView::pressed,
            m_delegate, &Internal::OpenDocumentsDelegate::handlePressed);
}

void OpenDocumentsTreeView::setModel(QAbstractItemModel *model)
{
    Utils::TreeView::setModel(model);
    header()->hide();
    header()->setStretchLastSection(false);
    header()->setSectionResizeMode(0, QHeaderView::Stretch);
    header()->setSectionResizeMode(1, QHeaderView::Fixed);
    header()->resizeSection(1, 16);
}

void OpenDocumentsTreeView::setCloseButtonVisible(bool visible)
{
    m_delegate->setCloseButtonVisible(visible);
}

bool OpenDocumentsTreeView::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this && event->type() == QEvent::KeyPress
            && currentIndex().isValid()) {
        QKeyEvent *ke = static_cast<QKeyEvent*>(event);
        if ((ke->key() == Qt::Key_Delete
                   || ke->key() == Qt::Key_Backspace)
                && ke->modifiers() == 0) {
            emit closeActivated(currentIndex());
        }
    } else if (obj == viewport()
             && event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent * me = static_cast<QMouseEvent*>(event);
        if (me->button() == Qt::MiddleButton
                && me->modifiers() == Qt::NoModifier) {
            QModelIndex index = indexAt(me->pos());
            if (index.isValid()) {
                emit closeActivated(index);
                return true;
            }
        }
    }
    return false;
}

} // namespace Core
