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
    Q_PROPERTY(int activePlugin READ activePlugin WRITE setActivePlugin NOTIFY activePluginChanged)
public:
    EstimationMode();
    ~EstimationMode();

    void activated();
    void initPlugins();
    int activePlugin() const { return m_activePlugin; }

public slots:
    void setActivePlugin(int pos)
    {
        if (m_activePlugin != pos) {
            m_activePlugin = pos;
            emit activePluginChanged(pos);
        }
    }
    void pluginAdded(QObject *obj);

signals:
    void activePluginChanged(int pos);

private:
    void schedulePluginAdded(QObject*);

    QWidget *m_matchPage;
    QWidget *m_modeWidget;
    int m_activePlugin;
};

} // Internal
} // Estimation

#endif // ESTIMATIONMODE_H
