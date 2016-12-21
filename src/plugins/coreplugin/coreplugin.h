
#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include <extensionsystem/iplugin.h>

#include "core_global.h"

QT_BEGIN_NAMESPACE
class QMenu;
class QMainWindow;
QT_END_NAMESPACE


namespace Utils {
class Theme;
}

namespace Core {

class FindPlugin;

namespace Internal {

using namespace ExtensionSystem;

class MainWindow;
class EditMode;

class CorePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ru.bonaka.app.plugin" FILE "core.json")

public:
    CorePlugin();
    ~CorePlugin();

    bool initialize(const QStringList &arguments, QString *errorMessage = 0);
    void extensionsInitialized();
    bool delayedInitialize();
    ShutdownFlag aboutToShutdown();

private slots:

private:
    void parseArguments(const QStringList & arguments);
    MainWindow *m_mainWindow;
};

} // namespace Internal
} // namespace Core

#endif // COREPLUGIN_H
