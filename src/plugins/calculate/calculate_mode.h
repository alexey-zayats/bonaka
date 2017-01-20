#ifndef CALCULATE_MODE_H
#define CALCULATE_MODE_H

#include <QtCore/QObject>

#include <coreplugin/imode.h>
#include <coreplugin/id.h>

using namespace Core;

namespace Core {
class IMatchBoard;
}

namespace Calculate {

namespace Internal {

class CalculateMode : public Core::IMode
{
    Q_OBJECT

public:
    CalculateMode(QWidget *parent = 0);
    ~CalculateMode();

};

} // Internal
} // Calculate

#endif // CALCULATE_MODE_H
