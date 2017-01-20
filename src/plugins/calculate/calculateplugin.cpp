
#include "calculateplugin.h"

#include "calculate_constants.h"
#include "calculate_mode.h"
#include "calculatewindow.h"

#include <coreplugin/modemanager.h>

using namespace Calculate;
using namespace Calculate::Internal;

Q_LOGGING_CATEGORY(calculateLog, "bonaka.plugins.calculate")

 inline void initCalculateResource() { Q_INIT_RESOURCE(calculate); }

CalculatePlugin::CalculatePlugin() :
    m_calculateMode(0),
    m_calculateWindow(0)
{
    initCalculateResource();
}

CalculatePlugin::~CalculatePlugin()
{
    delete m_calculateWindow;
}

bool CalculatePlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorMessage);

    m_calculateWindow = new CalculateWindow;
    m_calculateMode = new CalculateMode(m_calculateWindow);

    addAutoReleasedObject (m_calculateMode);
    return true;
}

void CalculatePlugin::extensionsInitialized()
{
    ModeManager::activateMode(Id(Constants::MODE_CALCULATE));
}

bool CalculatePlugin::delayedInitialize()
{
    return true;
}

ExtensionSystem::IPlugin::ShutdownFlag CalculatePlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}
