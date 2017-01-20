
#include "registry.h"
#include "registry_mode.h"
#include "registry_constants.h"

#include <coreplugin/modemanager.h>

using namespace Registry;
using namespace Registry::Internal;

Q_LOGGING_CATEGORY(registryLog, "bonaka.plugins.registry")

RegistryPlugin::RegistryPlugin() :
    m_registryMode(0)
{
}

RegistryPlugin::~RegistryPlugin()
{
}

bool RegistryPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);

    m_registryMode = new RegistryMode;
    addAutoReleasedObject (m_registryMode);
    return true;
}

void RegistryPlugin::extensionsInitialized()
{
    ModeManager::activateMode(Id(Constants::MODE_REGISTRY));
}

bool RegistryPlugin::delayedInitialize()
{
    return true;
}

ExtensionSystem::IPlugin::ShutdownFlag RegistryPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}
