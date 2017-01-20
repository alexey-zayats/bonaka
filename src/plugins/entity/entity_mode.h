#ifndef ENTITY_MODE_H
#define ENTITY_MODE_H

#include <QtCore/QObject>

#include <coreplugin/imode.h>
#include <coreplugin/id.h>

using namespace Core;

namespace Entity {

namespace Internal {

class EntityMode : public Core::IMode
{
    Q_OBJECT

public:
    EntityMode();
    ~EntityMode();

private:
    QWidget *m_modeWidget;
    QWidget *m_innerPage;
};

} // Internal
} // Entity

#endif // ENTITY_MODE_H
