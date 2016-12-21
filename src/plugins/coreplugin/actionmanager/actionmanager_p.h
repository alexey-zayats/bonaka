#ifndef ACTIONMANAGERPRIVATE_H
#define ACTIONMANAGERPRIVATE_H

#include <coreplugin/actionmanager/command_p.h>
#include <coreplugin/actionmanager/actioncontainer_p.h>
#include <coreplugin/icontext.h>

#include <QMap>
#include <QHash>
#include <QMultiHash>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QLabel;
class QSettings;
QT_END_NAMESPACE

namespace Core {

namespace Internal {

class Action;
class ActionContainerPrivate;
class MainWindow;

class ActionManagerPrivate : public QObject
{
    Q_OBJECT

public:
    typedef QHash<Id, Action *> IdCmdMap;
    typedef QHash<Id, ActionContainerPrivate *> IdContainerMap;

    ~ActionManagerPrivate();

    void setContext(const Context &context);
    bool hasContext(int context) const;

    void saveSettings();
    void saveSettings(Action *cmd);

    void showShortcutPopup(const QString &shortcut);
    bool hasContext(const Context &context) const;
    Action *overridableAction(Id id);

    void readUserSettings(Id id, Action *cmd);

    void containerDestroyed();
    void actionTriggered();

    IdCmdMap m_idCmdMap;

    IdContainerMap m_idContainerMap;

    Context m_context;

    bool m_presentationModeEnabled = false;
};

} // namespace Internal
} // namespace Core

#endif // ACTIONMANAGERPRIVATE_H
