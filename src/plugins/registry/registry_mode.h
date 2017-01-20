#ifndef REGISTRY_MODE_H
#define REGISTRY_MODE_H

#include <QtCore/QObject>

#include <coreplugin/imode.h>
#include <coreplugin/id.h>

using namespace Core;

namespace Registry {

namespace Internal {

class RegistryMode : public Core::IMode
{
    Q_OBJECT

public:
    RegistryMode();
    ~RegistryMode();

private:
    QWidget *m_modeWidget;
    QWidget *m_innerPage;
};

} // Internal
} // Registry

#endif // REGISTRY_MODE_H
