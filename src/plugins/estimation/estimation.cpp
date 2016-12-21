
#include "estimation.h"
#include "estimationmode.h"
#include "estimationconstants.h"

#include <coreplugin/modemanager.h>

using namespace Estimation;
using namespace Estimation::Internal;

Q_LOGGING_CATEGORY(estimationLog, "bonaka.plugins.estimation")

EstimationPlugin::EstimationPlugin() :
    m_estimationMode(0)
{
}

EstimationPlugin::~EstimationPlugin()
{
}

bool EstimationPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);

    m_estimationMode = new EstimationMode;
    addAutoReleasedObject (m_estimationMode);
    return true;
}

void EstimationPlugin::extensionsInitialized()
{
    ModeManager::activateMode(Id(Constants::MODE_ESTIMATION));
}

bool EstimationPlugin::delayedInitialize()
{
    return true;
}

ExtensionSystem::IPlugin::ShutdownFlag EstimationPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}
