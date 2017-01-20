#include "boilerwidget.h"

#include "boiler.h"
#include "scum.h"

#include "treemodelcompleter.h"

#include <utils/styledbar.h>
#include <utils/lineedit.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QRadioButton>
#include <QFile>
#include <QIODevice>
#include <QTextCodec>
#include <QStandardItemModel>
#include <QTreeView>
#include <QHeaderView>
#include <QSignalMapper>
#include <QCompleter>
#include <QFormLayout>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QStackedWidget>
#include <QStringListModel>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QGridLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QScrollArea>
#include <qmath.h>

using namespace Utils;

namespace Calculate {

BoilerWidget::BoilerWidget(QWidget *parent) :
    QWidget(parent),
    boilerType(0),
    boilerKind(0)
{
    baseLayoutRow = 0;

    setBackgroundRole(QPalette::Base);

    auto styledBar = new StyledBar(this); // The black blob on top of the side bar
    styledBar->setObjectName("BoilerStyledBar");

    selectorView = new QWidget(this);
    selectorView->setObjectName("BoilerSelector");
    selectorView->setWindowTitle(tr("Расчет котлов"));
    selectorView->setAutoFillBackground(true);

    initCompleters();

    initWaterBoiler();
    initSteamBoiler();

    initScumRef();

    auto scrollArea = new QScrollArea(this);
    scrollArea->setAutoFillBackground(true);
    scrollArea->setWidget(selectorView);
    scrollArea->setVisible(true);
    scrollArea->setWidgetResizable(true);

    auto activeLabel = new QLabel(tr("Расчет котлов"));
    QFont font = activeLabel->font();
    font.setBold(true);
    font.setPointSizeF(font.pointSizeF() * 1.2);
    activeLabel->setFont(font);

    auto innerLayout = new QVBoxLayout;
    innerLayout->addWidget(activeLabel);
    innerLayout->addWidget(scrollArea);

    baseLayout = new QGridLayout(selectorView);

    initSource();
    initMathView();

    baseLayout->setColumnStretch(2, 100);
    baseLayout->setColumnStretch(3, 100);

    auto selectorLayout = new QVBoxLayout(this);
    selectorLayout->setContentsMargins(0, 0, 0, 0);
    selectorLayout->addWidget(styledBar);
    selectorLayout->addLayout(innerLayout);

    boilerTypeChanged(1);
    emit scumCombo->activated(0);
}

void BoilerWidget::initCompleters()
{
    waterCompleter = new QCompleter(this);
    waterCompleter->setCompletionMode(QCompleter::PopupCompletion);
    waterCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    waterCompleter->setCompletionColumn(0);
    waterCompleter->setMaxVisibleItems(50);

    steamCompleter = new QCompleter(this);
    steamCompleter->setCompletionMode(QCompleter::PopupCompletion);
    steamCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    steamCompleter->setCompletionColumn(0);
    steamCompleter->setMaxVisibleItems(50);

    scumCompleter = new QCompleter(this);
    scumCompleter->setCompletionMode(QCompleter::PopupCompletion);
    scumCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    scumCompleter->setCompletionColumn(0);
    scumCompleter->setMaxVisibleItems(50);

//    connect(scumCompleter, SIGNAL(activated(QString)), this, SLOT(setScumSpace(QString)));

    hpCompleter = new TreeModelCompleter(this);
    hpCompleter->setModel(treeModelFromFile(":/calculate/reference/heating-period.txt", hpCompleter));
    hpCompleter->setSeparator(QLatin1String("/"));
    hpCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    hpCompleter->setCompletionMode(QCompleter::PopupCompletion);

    connect(hpCompleter, SIGNAL(activated(QString)), this, SLOT(setYearDays(QString)));
}

void BoilerWidget::setScumSpace(const int idx)
{
    QString text = scumCombo->itemText(idx);
    Scum *s = scumHash->value(text);
    QDoubleSpinBox* in_Lscum = selectorView->findChild<QDoubleSpinBox *>("in_Lscum");
    in_Lscum->setMinimum(s->down());
    in_Lscum->setMaximum(s->up());
    in_Lscum->setValue(s->median());
}

void BoilerWidget::claculateClicked()
{

    QFont font;
    font.setBold(true);
    font.setPointSizeF(font.pointSizeF() * 1.2);

    QDoubleSpinBox* in_Qch = selectorView->findChild<QDoubleSpinBox *>("in_Qch"); // Теплота сгорания топлива
    QDoubleSpinBox* in_Ct = selectorView->findChild<QDoubleSpinBox *>("in_Ct");   // Стоимость топлива
    QDoubleSpinBox* in_SigmaScum = selectorView->findChild<QDoubleSpinBox *>("in_SigmaScum"); // Толщина слоя накипи
    QDoubleSpinBox* in_Lscum = selectorView->findChild<QDoubleSpinBox *>("in_Lscum"); // Теплопроводность накпипи
    QDoubleSpinBox* in_Qcp = selectorView->findChild<QDoubleSpinBox *>("in_Qcp"); // Производительность N
    QDoubleSpinBox* in_Dnp = selectorView->findChild<QDoubleSpinBox *>("in_Dnp"); // Производительность N
    QSpinBox* in_qcp = selectorView->findChild<QSpinBox *>("in_qcp"); // Производительность %
    QSpinBox* in_ncp = selectorView->findChild<QSpinBox *>("in_ncp"); // Режим работы часы
    QSpinBox* in_D = selectorView->findChild<QSpinBox *>("in_D"); // Режим работы дни
    QDoubleSpinBox* in_tpv = selectorView->findChild<QDoubleSpinBox *>("in_tpv"); // t воды на входе

    qreal Qch = in_Qch->value();
    qreal Ct = in_Ct->value();
    qreal SigmaScum = in_SigmaScum->value();
    qreal Lscum = in_Lscum->value();
    qreal Qcp = boilerType == 1 ? in_Qcp->value() : 0;
    qreal Dnp = boilerType == 2 ? in_Dnp->value() : 0;
    qreal qcp = in_qcp->value();
    qreal ncp = in_ncp->value();
    qreal D = in_D->value();
    qreal tpv = in_tpv->value();

    bool isRequiredEmpty = false;

    in_Qch->setStyleSheet("border: 1px solid " + QString(Qch == 0 ? "red" : "gray"));
    in_Ct->setStyleSheet("border: 1px solid " + QString(Ct == 0 ? "red" : "gray"));

    in_SigmaScum->setStyleSheet("border: 1px solid " + QString(SigmaScum == 0 ? "red" : "gray"));
    in_Lscum->setStyleSheet("border: 1px solid " + QString(Lscum == 0 ? "red" : "gray"));

    if ( boilerType == 1 ) {
        in_Qcp->setStyleSheet("border: 1px solid " + QString(qcp == 0 && Qcp == 0 ? "red" : "gray"));
        in_qcp->setStyleSheet("border: 1px solid " + QString(qcp == 0 && Qcp == 0 ? "red" : "gray"));
    } else {
        // boilerType == 2
        in_Dnp->setStyleSheet("border: 1px solid " + QString(qcp == 0 && Dnp == 0 ? "red" : "gray"));
        in_qcp->setStyleSheet("border: 1px solid " + QString(qcp == 0 && Dnp == 0 ? "red" : "gray"));
    }

    in_ncp->setStyleSheet("border: 1px solid " + QString(ncp == 0 && D == 0 ? "red" : "gray"));
    in_D->setStyleSheet("border: 1px solid " + QString(ncp == 0 && D == 0 ? "red" : "gray"));

    if ( Qch == 0) isRequiredEmpty = true;
    if ( Ct == 0) isRequiredEmpty = true;
    if ( SigmaScum == 0) isRequiredEmpty = true;
    if ( Lscum == 0 ) isRequiredEmpty = true;
    if ( boilerType == 1 ) {
        if ( qcp == 0 && Qcp == 0 ) isRequiredEmpty = true;
    } else {
        if ( qcp == 0 && Dnp == 0 ) isRequiredEmpty = true;
    }
    if ( ncp == 0 && D == 0 ) isRequiredEmpty = true;

    if ( isRequiredEmpty )
        return;

    mathWidget->show();

    qreal k0, k1, k2, k3, k4, k5;
    qreal R_Scum = 0.0,
            kpd_100_scum = 0.0,
            kpd_50_scum = 0.0;

    // Термическое сопротивление накипи
    k0 = 0.001;
    k1 = 0.03;

    R_Scum = (SigmaScum * k0) / Lscum;

    // 1. Термическое сопротивление накипи
    QLabel* l_R_Scum = selectorView->findChild<QLabel *>("l_R_Scum");
    l_R_Scum->setStyleSheet("color: black");

    l_R_Scum->setText(
        "R<sub>нак</sub> = ( " + QString(QChar(0x03B4)) + "<sub>нак</sub> * " + QString::number(k0) + " ) / " +
                QString(QChar(0x03BB)) + "<sub>нак</sub> <br/>"
        +
        "R<sub>нак</sub> = ( " + QString::number(SigmaScum) + " * " + QString::number(k0) + " ) / "
            + QString::number(Lscum)
            + " = <b>" + QString::number(R_Scum) + "</b>"
    );

    if ( R_Scum > k1 ) {
        l_R_Scum->setStyleSheet("color: red");
        l_R_Scum->setText(
            l_R_Scum->text() + "<br/>котел неработоспособен - cлишком большая температура уходящих газов"
        );
    }

    // 2. КПД котла с накипью при режиме 50-ти процентной нагрузки с накипью
    QLabel* l_kpd_50_scum = selectorView->findChild<QLabel *>("l_kpd_50_scum");
    switch (boilerType) {
    case 1: {
        // КПД котла с накипью при режиме 50% процентной нагрузки с накипью
        k0 = -5 * qPow(10, -6);
        k1 = 592943;
        k2 = 25869;
        k3 = 189.61;
        k4 = 95.211;
        kpd_50_scum = k0 * qPow(R_Scum, 4) + k1 * qPow(R_Scum, 3) - k2 * qPow(R_Scum, 2) - k3 * R_Scum + k4;

        l_kpd_50_scum->setText(

          QString(QChar(0x03b7)) + "<sup>50</sup><sub>ка.НАК</sub> = "
                    + QString::number(k0) + " * R<sub>нак</sub><sup>4</sup>"
                    + " + " + QString::number(k1) + " * R<sub>нак</sub><sup>3</sup>"
                    + " - " + QString::number(k2) + " * R<sub>нак</sub><sup>2</sup>"
                    + " - " + QString::number(k3) + " * R<sub>нак</sub>"
                    + " + " + QString::number(k4)

          + "<br/>" +

          QString(QChar(0x03b7)) + "<sup>50</sup><sub>ка.НАК</sub> = "
                    + QString::number(k0) + " * " + QString::number( qPow(R_Scum, 4) )
                    + " + " + QString::number(k1) + " * " + QString::number( qPow(R_Scum, 3) )
                    + " - " + QString::number(k2) + " * " + QString::number( qPow(R_Scum, 2) )
                    + " - " + QString::number(k3) + " * " + QString::number( R_Scum )
                    + " + " + QString::number( k4 )  +
            " = <b>" +  QString::number(kpd_50_scum) + "</b>"
        );
        break;
    }
    case 2:
        k0 = 22853;
        k1 = 854.9;
        k2 = 89.607;
        kpd_50_scum = k0 * qPow(R_Scum, 2) - k1 * R_Scum + k2;
        l_kpd_50_scum->setText(

          QString(QChar(0x03b7)) + "<sup>50</sup><sub>ка.НАК</sub> = "
                    + QString::number(k0) + " * R<sub>нак</sub><sup>2</sup>"
                    + " - " + QString::number(k1) + " * R<sub>нак</sub>"
                    + " + " + QString::number(k2)

          + "<br/>" +

          QString(QChar(0x03b7)) + "<sup>50</sup><sub>ка.НАК</sub> = "
                    + QString::number(k0) + " * " + QString::number( qPow(R_Scum, 2) )
                    + " - " + QString::number(k1) + " * " + QString::number( R_Scum )
                    + " + " + QString::number(k2)  +
            " = <b>" +  QString::number(kpd_50_scum) + "</b>"
        );
        break;
    }

    // 3. КПД котла с накипью при режиме 100 процентной нагрузки с накипью
    QLabel* l_kpd_100_scum = selectorView->findChild<QLabel *>("l_kpd_100_scum");

    k0 = boilerType == 1 ? 7187.8 : 42466;
    k1 = boilerType == 1 ? 1434.2 : 2513.1;
    k2 = boilerType == 1 ? 94.515 : 89.405;

    kpd_100_scum = k0 * qPow(R_Scum, 2) - k1 * R_Scum + k2;
    l_kpd_100_scum->setText(
      QString(QChar(0x03b7)) + "<sup>100</sup><sub>ка.НАК</sub> = "
                + QString::number(k0) + " * R<sub>нак</sub><sup>2</sup>"
                + " - " + QString::number(k1) + " * R<sub>нак</sub>"
                + " + " + QString::number(k2)

      + "<br/>" +

      QString(QChar(0x03b7)) + "<sup>100</sup><sub>ка.НАК</sub> = "
                + QString::number(k0) + " * " + QString::number( qPow(R_Scum, 2) )
                + " - " + QString::number(k1) + " * " + QString::number( R_Scum )
                + " + " + QString::number(k2)
                + " = <b>" +  QString::number(kpd_100_scum) + "</b>"
    );

    QLabel *l_qcp = selectorView->findChild<QLabel *>("l_qcp");

    // 4. Нагрузка в среднем режиме в процентах от максимальной
    if ( qcp == 0 ) {
        k0 = boilerType == 1 ? 4.65 : 10;
        k1 = boilerType == 1 ? Qcp : Dnp;
        k2 = 100;

        qcp = (k1 / k0) * k2;

        l_qcp->setText(
            "q<sub>СР</sub> = (Q<sub>ср</sub> / " + QString::number(k0) + ") * 100"
             + "<br/>" +
             "q<sub>СР</sub> = (" + QString::number(k1) + " / " + QString::number(k0) + ") * " + QString::number(k2) + " = <b>" + QString::number(qcp) + "</b>"
        );
    } else {
        l_qcp->setText( "q<sub>СР</sub> = " + QString::number(qcp) );
    }

    // 5. КПД котла в среднем режиме с накипью
    k0 = 50;
    qreal kpd_scum = ((kpd_100_scum - kpd_50_scum) / k0) * (qcp - k0) + kpd_50_scum;

    QLabel *l_kpd_with_scum = selectorView->findChild<QLabel *>("l_kpd_with_scum");
    l_kpd_with_scum->setText(
         QString(QChar(0x03b7)) + "<sup>ср</sup><sub>ка.НАК</sub> = "
                + "((" + QString(QChar(0x03b7)) + "<sup>100</sup><sub>ка.НАК</sub> " + " - " + QString(QChar(0x03b7)) + "<sup>50</sup><sub>ка.НАК</sub>) / " +QString::number(k0) + " ) "
                + " * (q<sub>СР</sub> - " + QString::number(k0) + ") + " + QString(QChar(0x03b7)) + "<sup>50</sup><sub>ка.НАК</sub>"
         + "<br/>" +
         QString(QChar(0x03b7)) + "<sup>ср</sup><sub>ка.НАК</sub> = "
                + "((" + QString::number(kpd_100_scum) + " - " + QString::number(kpd_50_scum) + ") / " + QString::number(k0) + " ) "
                + " * (" + QString::number(qcp) + " - " + QString::number(k0) + ") + " + QString::number(kpd_50_scum)
                + " = <b>" + QString::number(kpd_scum) + "</b>"
    );

    // 6. КПД котла в среднем режиме без накипи
    k1 = boilerType == 1 ? 94.515 : 89.405;
    k2 = boilerType == 1 ? 95.211 : 89.607;
    qreal kpd = (( k1 - k2 ) / k0) * (qcp - k0) + k2;

    QLabel *l_kpd_without_scum = selectorView->findChild<QLabel *>("l_kpd_without_scum");
    l_kpd_without_scum->setText(
         QString(QChar(0x03b7)) + "<sup>ср</sup><sub>ка.</sub> = "
                + "([" + QString::number(k1) + " - " + QString::number(k2) + "] / 50 ) "
                + " * (q<sub>СР</sub> - " + QString::number(k0) + ") + " + QString::number(k2)
            + "<br/>" +
         QString(QChar(0x03b7)) + "<sup>ср</sup><sub>ка.</sub> = "
                + "([" + QString::number(k1) + " - " + QString::number(k2) + "] / " +QString::number(k0) + " ) "
                + " * " + QString::number(qcp) + " - " + QString::number(k0) + ") + " + QString::number(k2)
                + " = <b>" + QString::number(kpd) + "</b>"
    );

    // 7. Теплопроизвдительность в среднем режиме
    // l_heat_product
    QLabel *l_heat_product = selectorView->findChild<QLabel *>("l_heat_product");


    if (boilerType == 1) {
        l_heat_product->setText( QString::number(Qcp) );
    } else {
        k0 = 0.278;
        k1 = 2789;
        k2 = 4.19;
        k3 = 0.03;
        k4 = 830;
        k5 = 0.001;
        Qcp = (Dnp * k0 * (k1 - (tpv * k2) ) + k3 * Dnp * k0 * (k4 - tpv * k2) ) * k5;
        l_heat_product->setText(
            "Q<sub>ср</sub> = [ D<sub>нп</sub> * " + QString::number(k0) + " * ( " + QString::number(k1) + " - t<sub>пв</sub> * " + QString::number(k2) + " ) + " + QString::number(k3) + " * D<sub>нп</sub> * " + QString::number(k0) + " * ( " + QString::number(k4) + " - t<sub>пв</sub> * " + QString::number(k2) + " ) ] * "+ QString::number(k5)
                + "<br/>" +
            "Q<sub>ср</sub> = [ " + QString::number(Dnp) + " * " + QString::number(k0) + " * ( " + QString::number(k1) + " - " + QString::number(tpv) + " * " + QString::number(k2) + " ) + " + QString::number(k3) + " * " + QString::number(Dnp) + " * " + QString::number(k0) + " * ( " + QString::number(k4) + " - " + QString::number(tpv) + " * " + QString::number(k2) + " ) ] * "+ QString::number(k5)
                + " = " + QString::number(Qcp)
        );
    }

    // l_fuel_rate
    // 8. Расход топлива в котле без накипи
    k0 = 100;
    qreal fuelRate = (Qcp / (Qch * kpd) ) * k0;
    QLabel *l_fuel_rate = selectorView->findChild<QLabel *>("l_fuel_rate");
    l_fuel_rate->setText(
        "В<sub>Т</sub> = (Q<sub>ср</sub> / ( Q<sup>с</sup><sub>н</sub> * " +  QString(QChar(0x03b7)) + "<sup>ср</sup><sub>ка.</sub> ) ) * " + QString::number(k0)
            + "<br/>" +
        "В<sub>Т</sub> = (" + QString::number(Qcp) + " / ( " + QString::number(Qch) + " * " + QString::number(kpd) + " ) ) * " + QString::number(k0)
            + " = <b>" + QString::number(fuelRate) + "</b>"
    );

    // l_delta_kpd
    // 9. Увеличение КПД котла после очистки
    qreal deltaKpd = kpd - kpd_scum;
    QLabel *l_delta_kpd = selectorView->findChild<QLabel *>("l_delta_kpd");
    l_delta_kpd->setText(
        QString(QChar(0x03b7)) + "<sub>ка.</sub> = " + QString(QChar(0x03b7)) + "<sup>ср</sup><sub>ка.</sub> - " + QString(QChar(0x03b7)) + "<sup>ср</sup><sub>ка.НАК</sub>"
            + "<br/>" +
        QString(QChar(0x03b7)) + "<sub>ка.</sub> = " + QString::number(kpd) + " - " + QString::number(kpd_scum)
                + " = <b>" + QString::number(deltaKpd) + "</b>"

    );

    // Экономическая часть

    // QString(QChar(0x0394)) + "В<sub>Т</sub>
    // QString(QChar(0x0394)) + "В<sub>Т.ГОД</sub>
    // <b>Э<sub>ГОД</sub>
    // <b>Т<sub>ОК</sub>

    // 1. Уменьшение расхода топлива после очистки накипи
    QLabel *l_delta_fuel_rate = selectorView->findChild<QLabel *>("l_delta_fuel_rate");
    k0 = 100;
    qreal delta_fuel_rate = ( ( Qcp * k0 ) / (Qch * kpd_scum ) ) - fuelRate;
    l_delta_fuel_rate->setText(
        QString(QChar(0x0394)) + "В<sub>Т</sub> = ( ( Q<sub>ср</sub> * " + QString::number(k0) + " ) / ( Q<sup>с</sup><sub>н</sub> * " +  QString(QChar(0x03b7))  + "<sup>ср</sup><sub>ка.НАК</sub> ) ) - В<sub>Т</sub>"
            + "<br/>" +
        QString(QChar(0x0394)) + "В<sub>Т</sub> = ( (" + QString::number(Qcp) + " * " + QString::number(k0) + ") / ( " + QString::number(Qch) + " * " + QString::number(kpd_scum) + " ) ) - " + QString::number(fuelRate)
            + " = <b>" + QString::number(delta_fuel_rate) + "</b>"
    );

    // 2. Годовое уменьшение расхода топлива после очистки накипи
    QLabel *l_annual_delta_fuel_rate = selectorView->findChild<QLabel *>("l_annual_delta_fuel_rate");
    k0 = 3600;

    if (ncp == 0)
        ncp = D * 24;


    qreal annual_delta_fuel_rate = delta_fuel_rate * k0 * ncp;
    l_annual_delta_fuel_rate->setText(
        QString(QChar(0x0394)) + "В<sub>Т.ГОД</sub> = " + QString(QChar(0x0394)) + "В<sub>Т</sub> * " + QString::number(k0) + " * n<sub>ср</sub>"
            + "<br/>" +
        QString(QChar(0x0394)) + "В<sub>Т.ГОД</sub> = " + QString::number(delta_fuel_rate) + " * " + QString::number(k0) + " * " + QString::number(ncp)
            + " = <b>" + QString::number(annual_delta_fuel_rate) + "</b>"
    );

    // 3. Годовая экономия
    QLabel *l_annual_savings = selectorView->findChild<QLabel *>("l_annual_savings");
    k0 = 0.001;
    qreal annual_savings = annual_delta_fuel_rate * Ct * k0;
    l_annual_savings->setText(
        "Э<sub>ГОД</sub> = " + QString(QChar(0x0394)) + "В<sub>Т.ГОД</sub> * Ц<sub>Т</sub> * " + QString::number(k0)
            + "<br/>" +
        "Э<sub>ГОД</sub> = " + QString::number(annual_delta_fuel_rate) + " * " +  QString::number(Ct) +  " * " + QString::number(k0)
            + " = <b>" + QString::number(annual_savings) + "</b>"
    );

    // 4. Срок окупаемости очистки
    QLabel *l_payback_period = selectorView->findChild<QLabel *>("l_payback_period");
    l_payback_period->setText(
        QString("Т<sub>ОК</sub> = Э<sub>ГОД</sub> / З<sub>ОЧ</sub><br/>") +
        "Т<sub>ОК</sub> = " + QString::number(annual_savings) + " / ? = <b>?</b>"
    );

}

void BoilerWidget::initMathView()
{
    mathWidget = new QWidget(this);
    mathWidget->hide();

    baseLayout->addWidget(mathWidget, baseLayoutRow++, 0, 1, 3);

    auto innerLayout = new QVBoxLayout( mathWidget );
    innerLayout->setSpacing(2);
    innerLayout->setMargin(2);

    auto label = new QLabel("<b>Результат расчета</b>");
    innerLayout->addWidget(label);

    label = new QLabel("1. <b>R<sub>НАК</sub>, (м<sup>2</sup>*К)/Вт</b> - <i>термическое сопротивление накипи</i>");
    innerLayout->addWidget(label);

    label = new QLabel("");
    label->setObjectName("l_R_Scum");
    innerLayout->addWidget(label);
    innerLayout->addSpacing(10);

    label = new QLabel("2. <b>" + QString(QChar(0x03b7)) + "<sup>50</sup><sub>ка.НАК</sub>, %" +"</b> - <i>КПД котла с накипью при режиме 50% процентной нагрузки с накипью</i>");
    innerLayout->addWidget(label);

    label = new QLabel("");
    label->setObjectName("l_kpd_50_scum");
    innerLayout->addWidget(label);
    innerLayout->addSpacing(10);

    label = new QLabel("3. <b>" + QString(QChar(0x03b7)) + "<sup>100</sup><sub>ка.НАК</sub>, %</b> - <i>КПД котла с накипью при режиме 100% процентной нагрузки с накипью</i>");
    innerLayout->addWidget(label);

    label = new QLabel("");
    label->setObjectName("l_kpd_100_scum");
    innerLayout->addWidget(label);
    innerLayout->addSpacing(10);

    label = new QLabel("4. <b>q<sub>СР</sub>, %</b> - <i>нагрузка в среднем режиме в процентах от максимальной</i>");
    innerLayout->addWidget(label);

    label = new QLabel("");
    label->setObjectName("l_qcp");
    innerLayout->addWidget(label);
    innerLayout->addSpacing(10);

    label = new QLabel("5. <b>" + QString(QChar(0x03b7)) + "<sup>ср</sup><sub>ка.НАК</sub>, %</b> - <i>КПД котла в среднем режиме с накипью</i>");
    innerLayout->addWidget(label);

    label = new QLabel("");
    label->setObjectName("l_kpd_with_scum");
    innerLayout->addWidget(label);
    innerLayout->addSpacing(10);

    label = new QLabel("6. <b>" + QString(QChar(0x03b7)) + "<sup>ср</sup><sub>ка.</sub>, %</b> - <i>КПД котла в среднем режиме без накипи</i>");
    innerLayout->addWidget(label);

    label = new QLabel("");
    label->setObjectName("l_kpd_without_scum");
    innerLayout->addWidget(label);
    innerLayout->addSpacing(10);

    label = new QLabel("7. <b>Q<sub>ср</sub>, МВт</b> - <i>теплопроизвдительность в среднем режиме</i>");
    innerLayout->addWidget(label);

    label = new QLabel("");
    label->setObjectName("l_heat_product");
    innerLayout->addWidget(label);
    innerLayout->addSpacing(10);

    label = new QLabel("8. <b>В<sub>Т</sub>, м<sup>3</sup>/c</b> - <i>расход топлива в котле без накипи</i>");
    innerLayout->addWidget(label);

    label = new QLabel("");
    label->setObjectName("l_fuel_rate");
    innerLayout->addWidget(label);
    innerLayout->addSpacing(10);

    label = new QLabel("9. <b>" + QString(QChar(0x0394)) + QString(QChar(0x03b7)) + "<sub>ка.</sub>, %</b> - <i>увеличение КПД котла после очистки</i>");
    innerLayout->addWidget(label);

    label = new QLabel("");
    label->setObjectName("l_delta_kpd");
    innerLayout->addWidget(label);
    innerLayout->addSpacing(10);


    label = new QLabel("<b>Экономическая часть</b>");
    innerLayout->addWidget(label);

    label = new QLabel("1. <b>" + QString(QChar(0x0394)) + "В<sub>Т</sub>, м<sup>3</sup>/c</b> - <i>уменьшение расхода топлива после очистки накипи</i>");
    innerLayout->addWidget(label);

    label = new QLabel("");
    label->setObjectName("l_delta_fuel_rate");
    innerLayout->addWidget(label);
    innerLayout->addSpacing(10);

    label = new QLabel("2. <b>" + QString(QChar(0x0394)) + "В<sub>Т.ГОД</sub>, м<sup>3</sup>/c</b> - <i>годовое уменьшение расхода топлива после очистки накипи</i>");
    innerLayout->addWidget(label);

    label = new QLabel("");
    label->setObjectName("l_annual_delta_fuel_rate");
    innerLayout->addWidget(label);
    innerLayout->addSpacing(10);

    label = new QLabel("3. <b>Э<sub>ГОД</sub>, т.р./год</b> - <i>годовая экономия</i>");
    innerLayout->addWidget(label);

    label = new QLabel("");
    label->setObjectName("l_annual_savings");
    innerLayout->addWidget(label);
    innerLayout->addSpacing(10);

    label = new QLabel("4. <b>Т<sub>ОК</sub>, лет</b> - <i>срок окупаемости очистки</i>");
    innerLayout->addWidget(label);

    label = new QLabel("");
    label->setObjectName("l_payback_period");
    innerLayout->addWidget(label);
    innerLayout->addSpacing(10);

}

void BoilerWidget::initSource()
{
    auto btnGrp = new QButtonGroup();
    auto grpBox = new QGroupBox(selectorView);

    waterRadio = new QRadioButton(selectorView);
    steamRadio = new QRadioButton(selectorView);

    btnGrp->addButton(waterRadio, 1);
    btnGrp->addButton(steamRadio, 2);

    connect(btnGrp, SIGNAL(buttonClicked(int)),
            this, SLOT(boilerTypeChanged(int)));

    auto *vbox = new QVBoxLayout();
    vbox->addWidget(waterRadio);
    vbox->addWidget(steamRadio);
    vbox->addStretch(1);

    grpBox->setLayout(vbox);

    waterRadio->setText("Водогрейный");
    waterRadio->setChecked(true);

    steamRadio->setText("Паровой");

    waterRadio->setAutoExclusive(true);
    steamRadio->setAutoExclusive(true);

    calculateButton = new QPushButton(selectorView);
    calculateButton->setText("Рассчитать");

    connect(calculateButton, SIGNAL(clicked(bool)), this, SLOT(claculateClicked()));

    baseLayout->addWidget(new QLabel("Тип котла"), baseLayoutRow, 0, 1, 2);
    baseLayout->addWidget(grpBox, baseLayoutRow, 2);
    baseLayout->addWidget(calculateButton, baseLayoutRow++, 3);

    waterCompleter->setModel(waterModel);

    auto treeView = new QTreeView;
    waterCompleter->setPopup(treeView);
    setTreeHeader(treeView);

    steamCompleter->setModel(steamModel);

    treeView = new QTreeView;
    steamCompleter->setPopup(treeView);
    setTreeHeader(treeView);

    boilerCombo = new QComboBox(selectorView);
    boilerCombo->setEditable(true);
    boilerCombo->setAutoCompletion(true);
    boilerCombo->setStyleSheet("border-color: gray");

    connect(boilerCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(boilerKindChanged(int)));

    baseLayout->addWidget(new QLabel("Выбор котла"), baseLayoutRow, 0, 1, 2);
    baseLayout->addWidget(boilerCombo, baseLayoutRow++, 2, 1, 2);


    fuelCombo = new QComboBox();
    fuelCombo->setStyleSheet("border-color: gray");
    baseLayout->addWidget(new QLabel("Выбор топлива"), baseLayoutRow, 0, 1, 2);
    baseLayout->addWidget(fuelCombo, baseLayoutRow++, 2, 1, 2);

    // -- топливо
    baseLayout->addWidget(new QLabel("<b>Топливо</b>"), baseLayoutRow++, 0, 1, 4);

    auto in_Qch = new QDoubleSpinBox(selectorView);
    in_Qch->setMaximum(999);
    in_Qch->setStyleSheet("border-color: gray");
    in_Qch->setSuffix(" (МДж/м3)");
    in_Qch->setObjectName("in_Qch");

    baseLayout->addWidget(new QLabel("Теплота сгорания"), baseLayoutRow, 0);
    baseLayout->addWidget(new QLabel("<b>Q<sup>c</sup><sub>н</sub></b>"), baseLayoutRow, 1);
    baseLayout->addWidget(in_Qch, baseLayoutRow++, 2, 1, 2);

    auto in_Ct = new QDoubleSpinBox(selectorView);
    in_Ct->setMaximum(99999);
    in_Ct->setStyleSheet("border-color: gray");
    in_Ct->setSuffix(" (руб/м3)");
    in_Ct->setObjectName("in_Ct");

    baseLayout->addWidget(new QLabel("Стоимость"), baseLayoutRow, 0);
    baseLayout->addWidget(new QLabel("<b>Ц<sub>т</sub</b>"), baseLayoutRow, 1);
    baseLayout->addWidget(in_Ct, baseLayoutRow++, 2, 1, 2);

    // -- накипь
    baseLayout->addWidget(new QLabel("<b>Накипь</b>"), baseLayoutRow++, 0, 1, 4);

    auto in_SigmaScum = new QDoubleSpinBox(selectorView);
    in_SigmaScum->setStyleSheet("border-color: gray");
    in_SigmaScum->setSuffix(" (мм)");
    in_SigmaScum->setObjectName("in_SigmaScum");

    baseLayout->addWidget(new QLabel("Толщина слоя"), baseLayoutRow, 0);
    baseLayout->addWidget(new QLabel("<b>" + QString(QChar(0x03B4)) + "<sub>нак</sub></b>"), baseLayoutRow, 1);
    baseLayout->addWidget(in_SigmaScum, baseLayoutRow++, 2, 1, 2);

//    treeView = new QTreeView;
//    scumCompleter->setPopup(treeView);
//    setTreeHeader(treeView);

    scumCombo = new QComboBox(selectorView);

    connect(scumCombo, SIGNAL(activated(int)), this, SLOT(setScumSpace(int)));

    scumCombo->setStyleSheet("border-color: gray");
    scumCombo->setModel(scumModel);

    auto in_Lscum = new QDoubleSpinBox(selectorView);
    in_Lscum->setReadOnly(true);
    in_Lscum->setStyleSheet("border-color: gray");
    in_Lscum->setSuffix(" (Вт/м*К)");
    in_Lscum->setObjectName("in_Lscum");

    auto hbox = new QHBoxLayout;
    hbox->addWidget(scumCombo);
    hbox->addWidget(in_Lscum);

    baseLayout->addWidget(new QLabel("Теплопроводность"), baseLayoutRow, 0);
    baseLayout->addWidget(new QLabel("<b>" + QString(QChar(0x03BB)) + "<sub>нак</sub></b>"), baseLayoutRow, 1);
    baseLayout->addLayout(hbox, baseLayoutRow++, 2, 1, 2);

    // -- котел
    baseLayout->addWidget(new QLabel("<b>Котел</b>"), baseLayoutRow++, 0, 1, 4);

    auto in_Qcp = new QDoubleSpinBox(selectorView);
    in_Qcp->setStyleSheet("border-color: gray");
    in_Qcp->setSuffix(" (МВт)");
    in_Qcp->setObjectName("in_Qcp");

    auto in_Dnp = new QDoubleSpinBox(selectorView);
    in_Dnp->setMaximum(999);
    in_Dnp->setStyleSheet("border-color: gray");
    in_Dnp->setSuffix(" (т/ч)");
    in_Dnp->setObjectName("in_Dnp");

    auto in_qcp = new QSpinBox(selectorView);
    in_qcp->setMaximum(100);
    in_qcp->setStyleSheet("border-color: gray");
    in_qcp->setSuffix(" (%)");
    in_qcp->setObjectName("in_qcp");


    baseLayout->addWidget(new QLabel("Производительность"), baseLayoutRow, 0, 2, 1);

    auto label = new QLabel("<b>Q<sub>ср</sub></b>");
    label->setObjectName("l_Qcp_Dnp_dim");
    baseLayout->addWidget(label, baseLayoutRow, 1);

    vbox = new QVBoxLayout();
    vbox->addWidget(new QLabel("для среднего режима"));
    vbox->addWidget(in_Qcp);
    vbox->addWidget(in_Dnp);
    baseLayout->addLayout(vbox, baseLayoutRow++, 2, 1, 2);

    baseLayout->addWidget(new QLabel("<b>q<sub>ср</sub></b>"), baseLayoutRow, 1);
    vbox = new QVBoxLayout();
    vbox->addWidget(new QLabel("для среднего режима в процентах от максимальной"));
    vbox->addWidget(in_qcp);
    baseLayout->addLayout(vbox, baseLayoutRow++, 2, 1, 2);


    // ---
    auto in_ncp = new QSpinBox(selectorView);
    in_ncp->setMaximum(9000);
    in_ncp->setStyleSheet("border-color: gray");
    in_ncp->setSuffix(" (часов/год)");
    in_ncp->setObjectName("in_ncp");

    auto in_D = new QSpinBox(selectorView);
    in_D->setStyleSheet("border-color: gray");
    in_D->setMaximum(365);
    in_D->setSuffix(" (дней/год)");
    in_D->setObjectName("in_D");


    auto hpLineEdit = new QLineEdit(selectorView);
    hpLineEdit->setStyleSheet("border-color: gray");
    hpLineEdit->setPlaceholderText("Укажите город эксплуатации (Страна/Регион/Город)");
    hpLineEdit->setCompleter(hpCompleter);

    hbox = new QHBoxLayout;
    hbox->addWidget(hpLineEdit);
    hbox->addWidget(in_D);


    baseLayout->addWidget(new QLabel("Режим работы"), baseLayoutRow, 0, 2, 1);
    baseLayout->addWidget(new QLabel("<b>n<sub>ср</sub></b>"), baseLayoutRow, 1);

    vbox = new QVBoxLayout;
    vbox->addWidget(new QLabel("Число часов работы котла в среднем режиме в год )"));
    vbox->addWidget(in_ncp);

    baseLayout->addLayout(vbox, baseLayoutRow++, 2, 1, 2);

    baseLayout->addWidget(new QLabel("<b>Д</b>"), baseLayoutRow, 1);

    vbox = new QVBoxLayout;
    vbox->addWidget(new QLabel("Выбор город, для определения числа дней отопительного периода по СНИП 23-01-99"));
    vbox->addLayout(hbox);
    vbox->addStretch(1);

    baseLayout->addLayout(vbox, baseLayoutRow++, 2, 1, 2);

    // ---
    auto in_tpv = new QDoubleSpinBox(selectorView);
    in_tpv->setMaximum(1000);
    in_tpv->setStyleSheet("border-color: gray");
    in_tpv->setSuffix(" (С)");
    in_tpv->setObjectName("in_tpv");

    label = new QLabel("<b>t</b> воды на входе", selectorView);
    label->setObjectName("l_in_tpv");
    baseLayout->addWidget(label, baseLayoutRow, 0);
    label = new QLabel("<b>t<sub>пв</sub></b>");
    label->setObjectName("l2_in_tpv");
    baseLayout->addWidget(label, baseLayoutRow, 1);
    baseLayout->addWidget(in_tpv, baseLayoutRow++, 2, 1, 2);
}

void BoilerWidget::setYearDays(const QString &text)
{
    QStringList paths = text.split("/");
    QString mark = paths.at(paths.size()-1);

    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(hpCompleter->model());

    QModelIndexList items = model->match( model->index(0, 0),
                Qt::DisplayRole,
                QVariant::fromValue(mark),
                2,
                Qt::MatchRecursive);

    if ( items.size() == 0 )
        return;

    if ( items.size() > 1 ) {
        qDebug() << "Found nore than one item: " << items.size();
        return;
    }

    QSpinBox *spin = selectorView->findChild<QSpinBox *>("in_D");
    spin->setValue(  items.at(0).data(Qt::UserRole + 1).toInt() );
}

void BoilerWidget::setTreeHeader(QTreeView *treeView)
{
    treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    return;
}

BoilerWidget::~BoilerWidget()
{
    delete waterBoilers;
    delete steamBoilers;
}

void BoilerWidget::boilerKindChanged(int idx)
{
    boilerKind = idx;

    QString mark("");
    int fuel = 0;

    QDoubleSpinBox *spin = selectorView->findChild<QDoubleSpinBox *>("in_tpv");
    QLabel *l = selectorView->findChild<QLabel *>("l_in_tpv");
    QLabel *l2 = selectorView->findChild<QLabel *>("l2_in_tpv");

    QDoubleSpinBox *in_Qcp = selectorView->findChild<QDoubleSpinBox *>("in_Qcp");
    QDoubleSpinBox *in_Dnp = selectorView->findChild<QDoubleSpinBox *>("in_Dnp");
    QLabel *l_Qcp_Dnp_dim = selectorView->findChild<QLabel *>("l_Qcp_Dnp_dim");

    mathWidget->hide();

    // TODO: убрать поиск конкретных котлов
    if (boilerType == 1) {
        int id = boilerCombo->findText("КВ-ГМ-4,65-150");
        if ( id == idx ) {
            return;
        }
        idx = id;
        boilerCombo->setCurrentIndex( idx );
        mark = waterModel->data( waterModel->index(idx, 0) ).toString();
        if (!mark.isEmpty())
            fuel = waterBoilers->value(mark)->fuel();
        spin->hide();
        l->hide();
        l2->hide();
        in_Qcp->show();
        in_Dnp->hide();
        l_Qcp_Dnp_dim->setText("<b>Q<sub>ср</sub></b>");
    } else {
        int id = boilerCombo->findText("ДКВР 10-13ГМ");
        if ( id == idx ) {
            return;
        }
        idx = id;
        boilerCombo->setCurrentIndex( idx );
        mark = steamModel->data( steamModel->index(idx, 0) ).toString();
        if (!mark.isEmpty())
            fuel = steamBoilers->value(mark)->fuel();
        spin->show();
        l->show();
        l2->show();
        in_Qcp->hide();
        in_Dnp->show();
        l_Qcp_Dnp_dim->setText("<b>D<sub>нп</sub></b>");
    }

    if ( fuel > 0 ) {
        fuelCombo->clear();
        fuelCombo->addItems( Boiler::fuelTitle(fuel) );
    }
}

void BoilerWidget::boilerTypeChanged(int idx)
{
    fuelCombo->clear();
    boilerType = idx;
    boilerCombo->setModel( idx == 1 ? waterModel : steamModel);
    boilerCombo->setCompleter( idx == 1 ? waterCompleter : steamCompleter);
}

double BoilerWidget::asDouble(const QString &p) {
    QString num = p;
    num = num.replace(',','.');
    return num.toDouble();
}

void BoilerWidget::initWaterBoiler()
{
    QFile file(":/calculate/reference/waterboiler.txt");
    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        qDebug() << file.errorString() << file.fileName();
        return;
    }

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));

    QByteArray data = file.readAll();
    QList<QByteArray> lines = data.split('\n');

    waterBoilers = new QHash<QString,WaterBoiler*>();
    waterModel = new QStandardItemModel(lines.size(), 5, waterCompleter);
    waterModel->setHeaderData(0, Qt::Horizontal, "Марка");
    waterModel->setHeaderData(1, Qt::Horizontal, "Производительность");
    waterModel->setHeaderData(2, Qt::Horizontal, "Давление");
    waterModel->setHeaderData(3, Qt::Horizontal, "Водяной объем");
    waterModel->setHeaderData(4, Qt::Horizontal, "Топливо");

    for( int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);
        QStringList vars = line.trimmed().split('\t');
        if (vars.size() < 5) continue;

        QString mark        = vars.at(0);
        double perfomance   = asDouble(vars.at(1));
        double pressure     = asDouble(vars.at(2));
        double waterVolume  = asDouble(vars.at(3));
        int fuel            = vars.at(4).toInt();

        WaterBoiler *boiler = new WaterBoiler(mark, perfomance,
                                              pressure, waterVolume, fuel);
        waterBoilers->insert(mark, boiler);

        waterModel->setData(waterModel->index(i, 0), mark);
        waterModel->setData(waterModel->index(i, 1), perfomance);
        waterModel->setData(waterModel->index(i, 2), pressure);
        waterModel->setData(waterModel->index(i, 3), waterVolume);
        waterModel->setData(waterModel->index(i, 4), Boiler::fuelTitle(fuel).join(","));
    }
}

void BoilerWidget::initSteamBoiler()
{
    QFile file(":/calculate/reference/steamboiler.txt");
    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        qDebug() << file.errorString() << file.fileName();
        return;
    }

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));

    QByteArray data = file.readAll();
    QList<QByteArray> lines = data.split('\n');

    steamBoilers = new QHash<QString,SteamBoiler*>();
    steamModel = new QStandardItemModel(lines.size(), 6, steamCompleter);

    steamModel->setHeaderData(0, Qt::Horizontal, "Марка");
    steamModel->setHeaderData(1, Qt::Horizontal, "Производительность");
    steamModel->setHeaderData(2, Qt::Horizontal, "Давление");
    steamModel->setHeaderData(3, Qt::Horizontal, "Водяной объем");
    steamModel->setHeaderData(4, Qt::Horizontal, "Паровой объем");
    steamModel->setHeaderData(5, Qt::Horizontal, "Топливо");

    for( int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);
        QStringList vars = line.trimmed().split('\t');
        if (vars.size() < 6) continue;

        QString mark        = vars.at(0);
        double perfomance   = asDouble(vars.at(1));
        double pressure     = asDouble(vars.at(2));
        double waterVolume  = asDouble(vars.at(3));
        double steamVolume  = asDouble(vars.at(4));
        int fuel = vars.at(5).toInt();

        SteamBoiler *boiler = new SteamBoiler(mark, perfomance, pressure,
                                              waterVolume, steamVolume, fuel);

        steamBoilers->insert(mark, boiler);

        steamModel->setData(steamModel->index(i, 0),  mark);
        steamModel->setData(steamModel->index(i, 1),  perfomance);
        steamModel->setData(steamModel->index(i, 2),  pressure);
        steamModel->setData(steamModel->index(i, 3),  waterVolume);
        steamModel->setData(steamModel->index(i, 4),  steamVolume);
        steamModel->setData(steamModel->index(i, 5),  Boiler::fuelTitle(fuel).join(","));

    }
}

void BoilerWidget::initScumRef()
{
    QFile file(":/calculate/reference/scumkind.txt");
    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        qDebug() << file.errorString() << file.fileName();
        return;
    }

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));

    QByteArray data = file.readAll();
    QList<QByteArray> lines = data.split('\n');

    scumHash = new QHash<QString,Scum*>();
    scumModel = new QStandardItemModel(lines.size(), 1, scumCompleter);
    scumModel->setHeaderData(0, Qt::Horizontal, "Вид накипи");

    for( int i = 0; i < lines.size(); i++) {
        QString line = lines.at(i);
        QStringList vars = line.trimmed().split('\t');
        if (vars.size() < 3) continue;

        QString kind    = vars.at(0);
        qreal down   = asDouble(vars.at(1));
        qreal up     = asDouble(vars.at(2));

        Scum *scum = new Scum(kind, down, up);
        scumHash->insert(kind, scum);

        scumModel->setData(scumModel->index(i, 0), kind);
    }
}

QAbstractItemModel *BoilerWidget::treeModelFromFile(const QString& fileName,
                                                    TreeModelCompleter *completer)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(completer);

    QStandardItemModel *model = new QStandardItemModel(completer);
    QVector<QStandardItem *> parents(10);
    parents[0] = model->invisibleRootItem();

    while (!file.atEnd()) {
        QString line = file.readLine();
        QString trimmedLine = line.trimmed();
        if (line.isEmpty() || trimmedLine.isEmpty())
            continue;

        QRegExp re("^\\s+");
        int nonws = re.indexIn(line);
        int level = 0;
        if (nonws == -1) {
            level = 0;
        } else {
            if (line.startsWith("\t")) {
                level = re.cap(0).length();
            } else {
                level = re.cap(0).length()/4;
            }
        }

        if (level+1 >= parents.size())
            parents.resize(parents.size()*2);

        QStandardItem *item = new QStandardItem;

        QStringList a = trimmedLine.split("\t");
        if ( a.at(a.size()-1).toInt() > 0 ) {
            item->setText(a.at(a.size()-2));
            item->setData(a.at(a.size()-1));
        } else {
            item->setText(trimmedLine);
        }

        parents[level]->appendRow(item);
        parents[level+1] = item;
    }

    return model;
}

}
