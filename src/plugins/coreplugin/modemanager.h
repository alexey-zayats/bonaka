
#ifndef MODEMANAGER_H
#define MODEMANAGER_H

#include <coreplugin/core_global.h>
#include <QObject>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

namespace Core {

class Id;
class IMode;

namespace Internal {
    class MainWindow;
    class TabWidget;
}

class CORE_EXPORT ModeManager : public QObject
{
    Q_OBJECT

public:
    static ModeManager *instance();

    static IMode *currentMode();
    static IMode *mode(Id id);

    static void addAction(QAction *action, int priority);
    static void addProjectSelector(QAction *action);

    static void activateMode(Id id);
    static void setFocusToCurrentMode();
    static bool isModeSelectorVisible();

public slots:
    static void setModeSelectorVisible(bool visible);

signals:
    void currentModeAboutToChange(Core::IMode *mode);

    // the default argument '=0' is important for connects without the oldMode argument.
    void currentModeChanged(Core::IMode *mode, Core::IMode *oldMode = 0);

private:
    explicit ModeManager(Internal::MainWindow *mainWindow, Internal::TabWidget *modeStack);
    ~ModeManager();

    static void init();

    void objectAdded(QObject *obj);
    void aboutToRemoveObject(QObject *obj);
    void currentTabAboutToChange(int index);
    void currentTabChanged(int index);
    void updateModeToolTip();
    void enabledStateChanged();

    friend class Core::Internal::MainWindow;
};

} // namespace Core

#endif // MODEMANAGER_H
