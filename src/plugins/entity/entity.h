

#ifndef ENTITY_PLUGIN_H
#define ENTITY_PLUGIN_H

#include <extensionsystem/iplugin.h>

#include "entity_global.h"

namespace Entity {

namespace Internal {

class EntityMode;

using namespace ExtensionSystem;

class EntityPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ru.bonaka.app.plugin" FILE "entity.json")

public:
    EntityPlugin();
    ~EntityPlugin();

    bool initialize(const QStringList &arguments, QString *errorMessage = 0);
    void extensionsInitialized();
    bool delayedInitialize();
    ShutdownFlag aboutToShutdown();

private slots:

private:
    EntityMode *m_entityMode;
};

} // namespace Internal
} // namespace Estimation

#endif // ENTITY_PLUGIN_H
