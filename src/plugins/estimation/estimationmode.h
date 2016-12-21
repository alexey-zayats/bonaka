#ifndef ESTIMATIONMODE_H
#define ESTIMATIONMODE_H

#include <QtCore/QObject>

#include <coreplugin/imode.h>
#include <coreplugin/id.h>

using namespace Core;

namespace Core {
class IMatchBoard;
}

namespace Estimation {

namespace Internal {

class EstimationMode : public Core::IMode
{
    Q_OBJECT

public:
    EstimationMode();
    ~EstimationMode();

private:
    QWidget *m_matchPage;
    QWidget *m_modeWidget;
};

} // Internal
} // Estimation

#endif // ESTIMATIONMODE_H
