#include "appmainwindow.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <QEvent>
#include <QCoreApplication>

namespace Utils {

/* The notification signal is delayed by using a custom event
 * as otherwise device removal is not detected properly
 * (devices are still present in the registry. */

class DeviceNotifyEvent : public QEvent {
public:
    explicit DeviceNotifyEvent(int id) : QEvent(static_cast<QEvent::Type>(id)) {}
};

AppMainWindow::AppMainWindow() :
        m_deviceEventId(QEvent::registerEventType(QEvent::User + 2))
{
}

void AppMainWindow::raiseWindow()
{
    setWindowState(windowState() & ~Qt::WindowMinimized);
    raise();
    activateWindow();
}

#ifdef Q_OS_WIN
bool AppMainWindow::event(QEvent *event)
{
    if (event->type() == m_deviceEventId) {
        event->accept();
        emit deviceChange();
        return true;
    }
    return QMainWindow::event(event);
}

bool AppMainWindow::winEvent(MSG *msg, long *result)
{
    if (msg->message == WM_DEVICECHANGE) {
        if (msg->wParam & 0x7 /* DBT_DEVNODES_CHANGED */) {
            *result = TRUE;
            QCoreApplication::postEvent(this, new DeviceNotifyEvent(m_deviceEventId));
        }
    }
    return false;
}
#endif

} // namespace Utils

