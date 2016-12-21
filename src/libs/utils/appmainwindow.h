#ifndef APPMAINWINDOW_H
#define APPMAINWINDOW_H

#include "utils_global.h"
#include <QMainWindow>

namespace Utils {

class UTILS_EXPORT AppMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    AppMainWindow();

public slots:
    void raiseWindow();

signals:
    void deviceChange();

#ifdef Q_OS_WIN
protected:
    virtual bool winEvent(MSG *message, long *result);
    virtual bool event(QEvent *event);
#endif

private:
    const int m_deviceEventId;
};

} // Utils

#endif // APPMAINWINDOW_H
