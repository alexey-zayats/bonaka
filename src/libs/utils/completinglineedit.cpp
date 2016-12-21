#include "completinglineedit.h"

#include <QAbstractItemView>
#include <QCompleter>
#include <QEvent>
#include <QKeyEvent>

namespace Utils {

CompletingLineEdit::CompletingLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

bool CompletingLineEdit::event(QEvent *e)
{
    // workaround for QTCREATORBUG-9453
    if (e->type() == QEvent::ShortcutOverride) {
        if (QCompleter *comp = completer()) {
            if (comp->popup() && comp->popup()->isVisible()) {
                QKeyEvent *ke = static_cast<QKeyEvent *>(e);
                if (ke->key() == Qt::Key_Escape && !ke->modifiers()) {
                    ke->accept();
                    return true;
                }
            }
        }
    }
    return QLineEdit::event(e);
}

void CompletingLineEdit::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Down && !e->modifiers()) {
        if (QCompleter *comp = completer()) {
            if (!comp->popup()->isVisible())
                comp->complete();
        }
    }
    return QLineEdit::keyPressEvent(e);
}

} // namespace Utils