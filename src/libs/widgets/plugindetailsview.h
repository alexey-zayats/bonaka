#ifndef PLUGINDETAILSVIEW_H_
#define PLUGINDETAILSVIEW_H_

#include "widgets_global.h"

#include <QWidget>

namespace ExtensionSystem {
    class PluginSpec;
}

namespace Widgets {

class Ui_PluginDetailsView;

class WIDGETS_EXPORT PluginDetailsView : public QWidget
{
    Q_OBJECT

public:
    PluginDetailsView(QWidget *parent = 0);
    ~PluginDetailsView();

    void update(ExtensionSystem::PluginSpec *spec);

private:
    Ui_PluginDetailsView *m_ui;
};

} // namespace ExtensionSystem

#endif
