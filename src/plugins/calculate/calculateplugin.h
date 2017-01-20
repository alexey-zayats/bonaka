

#ifndef CALCULATE_PLUGIN_H
#define CALCULATE_PLUGIN_H

#include <extensionsystem/iplugin.h>

#include "calculate_global.h"

namespace Calculate {

class CalculateWindow;

namespace Internal {

class CalculateMode;


using namespace ExtensionSystem;

class CalculatePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ru.bonaka.app.plugin" FILE "calculate.json")

public:
    CalculatePlugin();
    ~CalculatePlugin();

    bool initialize(const QStringList &arguments, QString *errorMessage = 0);
    void extensionsInitialized();
    bool delayedInitialize();
    ShutdownFlag aboutToShutdown();

private slots:

private:
    CalculateMode *m_calculateMode;
    CalculateWindow *m_calculateWindow;
};

} // namespace Internal
} // namespace Calculate

#endif // CALCULATE_PLUGIN_H
