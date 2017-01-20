#ifndef CALCULATE_WINDOW_H
#define CALCULATE_WINDOW_H

#include <QObject>
#include <utils/mainwindow.h>
#include "calculate_global.h"

class QPushButton;

namespace Utils {
    class ListWidget;
}

namespace Calculate
{

class BoilerCalculate;
class BoilerWidget;
class HeatExchanger;
class HeatVolume;

class CalculateWindow : public Utils::MainWindow
{
    Q_OBJECT
public:
    CalculateWindow();

public slots:
    void widgetChanged(int);

private:
    void setPanel(QWidget *panel);

    Utils::ListWidget *calcListWidget;
    BoilerWidget *boilerWidget;
    HeatExchanger *heatExchnagerWidget;
    HeatVolume *heatVolumeWidget;
    BoilerCalculate *boilerCalculate;

    QPushButton *boilerButton;
    QPushButton *boilerButton2;
    QPushButton *heatVolume;
    QPushButton *heatExchangeButton;
};
}

#endif // CALCULATE_WINDOW_H
