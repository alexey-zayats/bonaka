

#ifndef ESTIMATIONPLUGIN_H
#define ESTIMATIONPLUGIN_H

#include <extensionsystem/iplugin.h>

#include "estimation_global.h"

namespace Estimation {

namespace Internal {

class EstimationMode;

using namespace ExtensionSystem;

class EstimationPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ru.bonaka.app.plugin" FILE "estimation.json")

public:
    EstimationPlugin();
    ~EstimationPlugin();

    bool initialize(const QStringList &arguments, QString *errorMessage = 0);
    void extensionsInitialized();
    bool delayedInitialize();
    ShutdownFlag aboutToShutdown();

private slots:

private:
    EstimationMode *m_estimationMode;
};

} // namespace Internal
} // namespace Estimation

#endif // ESTIMATIONPLUGIN_H
