
#ifndef STATUSBARMANAGER_H
#define STATUSBARMANAGER_H

#include "minisplitter.h"

#include <QList>
#include <QObject>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

namespace Core {
namespace Internal {

class MainWindow;

class NonResizingSplitter : public MiniSplitter
{
    Q_OBJECT

public:
    explicit NonResizingSplitter(QWidget *parent);

protected:
    void resizeEvent(QResizeEvent *ev);
};

class StatusBarManager : public QObject
{
    Q_OBJECT

public:
    StatusBarManager(MainWindow *mainWnd);
    ~StatusBarManager();

    void init();
    void extensionsInitalized();
    void restoreSettings();

private:
    void objectAdded(QObject *obj);
    void aboutToRemoveObject(QObject *obj);
    void saveSettings();

    MainWindow *m_mainWnd;
    QSplitter *m_splitter;
    QList<QWidget *> m_statusBarWidgets;
};

} // namespace Internal
} // namespace Core

#endif // STATUSBARMANAGER_H
