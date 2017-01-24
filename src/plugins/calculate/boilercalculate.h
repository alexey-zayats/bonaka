#ifndef BOILERCALCULATE_H
#define BOILERCALCULATE_H

#include "ui_boilerform.h"

#include <QWidget>

class QComboBox;
class QStandardItemModel;

namespace Calculate {

class BoilerModels;

class BoilerCalculate : public QWidget, private Ui::BoilerForm
{
    Q_OBJECT
public:
    BoilerCalculate(QWidget *parent = 0);
    ~BoilerCalculate();

public slots:
    void calculateButton_released();
    void toFormButton_released();
    void boilerTypeChanged(int);

    void steamBoilerChanged(int);
    void waterBoilerChanged(int);

    void setWaterScumBorder(int);
    void setSteamScumBorder(int);

    void setWaterHeatDays(const QString &);
    void setSteamHeatDays(const QString &);

    void powerSpinEdited();
    void modeSpinEdited();

private:
    void init();
    void initCompleters();
    void showPane(int left, int right);
    void boilerChnaged (int index,
                        QComboBox *fuelCombo,
                        QStandardItemModel *model);
    void setScumBorder(int index,
                       QComboBox *scumTypeCombo,
                       QDoubleSpinBox *scumHeatSpin);
    void setHeatDays(const QString &text,
                     QSpinBox *scumHeatSpin);

    BoilerModels *m_boilerModels;
    int boilerType;
};

}

#endif // BOILERCALCULATE_H
