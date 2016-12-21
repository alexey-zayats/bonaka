#ifndef PLUGINERROROVERVIEW_H
#define PLUGINERROROVERVIEW_H

#include "widgets_global.h"

#include <QDialog>

QT_BEGIN_NAMESPACE
class QListWidgetItem;
QT_END_NAMESPACE

namespace Widgets {

class Ui_PluginErrorOverview;

class WIDGETS_EXPORT PluginErrorOverview : public QDialog
{
    Q_OBJECT

public:
    explicit PluginErrorOverview(QWidget *parent = 0);
    ~PluginErrorOverview();

private slots:
    void showDetails(QListWidgetItem *item);

private:
    Ui_PluginErrorOverview *m_ui;
};

} // Widgets

#endif // PLUGINERROROVERVIEW_H
