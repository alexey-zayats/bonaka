#ifndef WINDOWSUPPORT_H
#define WINDOWSUPPORT_H

#include "icontext.h"

#include <QObject>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QWidget;
QT_END_NAMESPACE

namespace Core {
namespace Internal {

class WindowList
{
public:
    static void addWindow(QWidget *window);
    static void removeWindow(QWidget *window);
    static void setActiveWindow(QWidget *window);
    static void setWindowVisible(QWidget *window, bool visible);

private:
    static void activateWindow(QAction *action);
    static void updateTitle(QWidget *window);

    static QMenu *m_dockMenu;
    static QList<QWidget *> m_windows;
    static QList<QAction *> m_windowActions;
    static QList<Id> m_windowActionIds;
};

class WindowSupport : public QObject
{
    Q_OBJECT
public:
    WindowSupport(QWidget *window, const Context &context);
    ~WindowSupport();

    void setCloseActionEnabled(bool enabled);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void toggleFullScreen();
    void updateFullScreenAction();

    QWidget *m_window;
    IContext *m_contextObject;
    QAction *m_minimizeAction;
    QAction *m_zoomAction;
    QAction *m_closeAction;
    QAction *m_toggleFullScreenAction;
    bool m_shutdown = false;
};

} // Internal
} // Core

#endif // WINDOWSUPPORT_H
