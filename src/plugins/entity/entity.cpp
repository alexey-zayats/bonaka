
#include "entity.h"
#include "entity_mode.h"
#include "entity_constants.h"

#include <coreplugin/modemanager.h>

using namespace Entity;
using namespace Entity::Constants;
using namespace Entity::Internal;

Q_LOGGING_CATEGORY(entityLog, "bonaka.plugins.entity")

EntityPlugin::EntityPlugin() :
    m_entityMode(0)
{
}

EntityPlugin::~EntityPlugin()
{
}

bool EntityPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);

    m_entityMode = new EntityMode;
    addAutoReleasedObject (m_entityMode);
    return true;
}

void EntityPlugin::extensionsInitialized()
{
    ModeManager::activateMode(Id(Constants::MODE_ENTITY));
}

bool EntityPlugin::delayedInitialize()
{
    return true;
}

ExtensionSystem::IPlugin::ShutdownFlag EntityPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}
