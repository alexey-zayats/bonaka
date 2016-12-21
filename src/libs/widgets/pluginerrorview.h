#ifndef PLUGINERRORVIEW_H
#define PLUGINERRORVIEW_H

#include "widgets_global.h"

#include <QWidget>

namespace ExtensionSystem {
    class PluginSpec;
}

namespace Widgets {

class Ui_PluginErrorView;

class WIDGETS_EXPORT PluginErrorView : public QWidget
{
    Q_OBJECT

public:
    PluginErrorView(QWidget *parent = 0);
    ~PluginErrorView();

    void update(ExtensionSystem::PluginSpec *spec);

private:
    Ui_PluginErrorView *m_ui;
};

} // namespace Widgets

#endif // PLUGINERRORVIEW_H
