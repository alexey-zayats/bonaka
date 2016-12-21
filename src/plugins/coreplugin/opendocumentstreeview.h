#ifndef OPENDOCUMENTSTREEVIEW_H
#define OPENDOCUMENTSTREEVIEW_H

#include "core_global.h"

#include <utils/itemviews.h>

namespace Core {
namespace Internal { class OpenDocumentsDelegate; }

class CORE_EXPORT OpenDocumentsTreeView : public Utils::TreeView
{
    Q_OBJECT
public:
    explicit OpenDocumentsTreeView(QWidget *parent = 0);

    void setModel(QAbstractItemModel *model);
    void setCloseButtonVisible(bool visible);

signals:
    void closeActivated(const QModelIndex &index);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Internal::OpenDocumentsDelegate *m_delegate;
};

} // namespace Core

#endif // OPENDOCUMENTSTREEVIEW_H
