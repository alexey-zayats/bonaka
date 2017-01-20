#ifndef REGISTRY_PLUGIN_H
#define REGISTRY_PLUGIN_H

#include <extensionsystem/iplugin.h>

#include "registry_global.h"

namespace Registry {

namespace Internal {

class RegistryMode;

using namespace ExtensionSystem;

class RegistryPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ru.bonaka.app.plugin" FILE "registry.json")

public:
    RegistryPlugin();
    ~RegistryPlugin();

    bool initialize(const QStringList &arguments, QString *errorMessage = 0);
    void extensionsInitialized();
    bool delayedInitialize();
    ShutdownFlag aboutToShutdown();

private slots:

private:
    RegistryMode *m_registryMode;
};

} // namespace Internal
} // namespace Registry

#endif // REGISTRY_PLUGIN_H
