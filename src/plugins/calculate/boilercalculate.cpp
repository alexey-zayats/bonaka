#include "boilercalculate.h"
#include "boilermodels.h"
#include "boiler.h"
#include "scum.h"
#include "treemodelcompleter.h"

#include <utils/styledbar.h>

#include <QDebug>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QCompleter>
#include <QTreeView>
#include <QDebug>
#include <QtMath>
#include <QCryptographicHash>

namespace Calculate {

BoilerCalculate::BoilerCalculate(QWidget *parent)
    : QWidget (parent),
      m_boilerModels(0),
      boilerType(0)
{
    setBackgroundRole(QPalette::Base);

    auto styledBar = new Utils::StyledBar(this); // The black blob on top of the side bar
    styledBar->setObjectName("BoilerStyledBar");

    auto selectorView = new QWidget(this);
    selectorView->setObjectName("BoilerSelector");
    selectorView->setWindowTitle(tr("Расчет котлов"));
    selectorView->setAutoFillBackground(true);

    auto selectorLayout = new QVBoxLayout(this);
    selectorLayout->setContentsMargins(0, 0, 0, 0);
    selectorLayout->addWidget(styledBar);
    selectorLayout->addWidget(selectorView);

    setLayout(selectorLayout);

    setupUi(selectorView);

    auto btnGrp = new QButtonGroup();

    btnGrp->addButton(waterRadio, 0);
    btnGrp->addButton(steamRadio, 1);

    connect(btnGrp, SIGNAL(buttonClicked(int)), this, SLOT(boilerTypeChanged(int)));

    connect(calculateButton, SIGNAL(released()), this, SLOT(calculateButton_released()));
    connect(toFormButton, SIGNAL(released()), this, SLOT(toFormButton_released()));


    connect(wPowerSpin1, SIGNAL(editingFinished()), this, SLOT(powerSpinEdited()));
    connect(wPowerSpin2, SIGNAL(editingFinished()), this, SLOT(powerSpinEdited()));

    connect(sPowerSpin1, SIGNAL(editingFinished()), this, SLOT(powerSpinEdited()));
    connect(sPowerSpin2, SIGNAL(editingFinished()), this, SLOT(powerSpinEdited()));

    connect(wModeSpin1, SIGNAL(editingFinished()), this, SLOT(modeSpinEdited()));
    connect(wModeSpin2, SIGNAL(editingFinished()), this, SLOT(modeSpinEdited()));

    connect(sModeSpin1, SIGNAL(editingFinished()), this, SLOT(modeSpinEdited()));
    connect(sModeSpin2, SIGNAL(editingFinished()), this, SLOT(modeSpinEdited()));

    m_boilerModels = new BoilerModels();

    init();
}

void BoilerCalculate::powerSpinEdited()
{
    qreal k0 = 0.0,
            k1 = 100,
            inValue = 0.0,
            outValue = 0.0;

    QString senderName = sender()->objectName();
    if ( senderName.startsWith("w") ) {
        k0 = 4.65;
    } else {
        k0 = 10;
    }

    if ( senderName.compare("wPowerSpin1") == 0 ) {
        inValue = wPowerSpin1->value();
        outValue = (inValue/k0)*k1;
        wPowerSpin2->setValue(outValue);
    }
    else if ( senderName.compare("wPowerSpin2") == 0 ) {
        inValue = wPowerSpin2->value();
        outValue = (inValue*k0)/k1;
        wPowerSpin1->setValue(outValue);
    }
    else if ( senderName.compare("sPowerSpin1") == 0 ) {
        inValue = sPowerSpin1->value();
        outValue = (inValue/k0)*k1;
        sPowerSpin2->setValue(outValue);
    }
    else if ( senderName.compare("sPowerSpin2") == 0 ) {
        inValue = sPowerSpin2->value();
        outValue = (inValue*k0)/k1;
        sPowerSpin1->setValue(outValue);
    }
}

void BoilerCalculate::modeSpinEdited()
{
    qreal k0 = 24;
    qreal value = 0.0;

    QSpinBox *src = 0,
             *dst = 0;

    bool isDivide = false;
    QString senderName = sender()->objectName();

    if ( senderName.compare("wModeSpin1") == 0 ) {
        isDivide = true;
        src = wModeSpin1;
        dst = wModeSpin2;
    }
    else if ( senderName.compare("wModeSpin2") == 0 ) {
        src = wModeSpin2;
        dst = wModeSpin1;
    }
    else if ( senderName.compare("sModeSpin1") == 0 ) {
        isDivide = true;
        src = sModeSpin1;
        dst = sModeSpin2;
    }
    else if ( senderName.compare("sModeSpin2") == 0 ) {
        src = sModeSpin2;
        dst = sModeSpin1;
    }

    if ( src && dst ) {
        value = src->value();
        dst->setValue( isDivide ? value / k0 : value * k0 );
    }
}

void BoilerCalculate::init()
{
    initCompleters();

    waterBoilerChanged(1);
    steamBoilerChanged(1);
    emit wScumTypeCombo->activated(0);
    emit sScumTypeCombo->activated(0);
    boilerTypeChanged(0);
    emit wPowerSpin2->editingFinished();
    emit sPowerSpin2->editingFinished();
}

void BoilerCalculate::steamBoilerChanged(int index)
{
    // TODO: убрать поиск конкретных котлов
    if ( 1 ) {
        int id = sBoilerCombo->findText("ДКВР 10-13ГМ");
        if ( id == index ) return;

        sBoilerCombo->setCurrentIndex( id );
        index = id;
    }

    boilerChnaged( index, sFuelCombo, m_boilerModels->steamBoilerModel());
}

void BoilerCalculate::waterBoilerChanged(int index)
{
    // TODO: убрать поиск конкретных котлов
    if ( 1 ) {
        int id = wBoilerCombo->findText("КВ-ГМ-4,65-150");
        if ( id == index ) return;

        wBoilerCombo->setCurrentIndex( id );
        index = id;
    }

    boilerChnaged( index, wFuelCombo, m_boilerModels->waterBoilerModel());
}

void BoilerCalculate::boilerChnaged (int index, QComboBox *fuelCombo, QStandardItemModel *model)
{
    QString mark = model->data(model->index(index, 0) ).toString();

    int fuel = 0;
    if (!mark.isEmpty())
        fuel = m_boilerModels->boiler(mark)->fuel();

    if ( fuel > 0 ) {
        fuelCombo->clear();
        fuelCombo->addItems( Boiler::fuelTitle(fuel) );
    }
}

void BoilerCalculate::setWaterScumBorder(int index)
{
    setScumBorder(index, wScumTypeCombo, wScumHeatSpin);
}

void BoilerCalculate::setSteamScumBorder(int index)
{
    setScumBorder(index, sScumTypeCombo, sScumHeatSpin);
}

void BoilerCalculate::setScumBorder(int index,
                                    QComboBox *scumTypeCombo,
                                    QDoubleSpinBox *scumHeatSpin)
{
    QString text = scumTypeCombo->itemText(index);

    scumHeatSpin->setMinimum(0);
    scumHeatSpin->setMaximum(5);
    scumHeatSpin->setValue(0.5);

    Scum *s = m_boilerModels->scum(text);
    if ( 0 != s ) {
        scumHeatSpin->setMinimum(s->down());
        scumHeatSpin->setMaximum(s->up());
        scumHeatSpin->setValue(s->median());
    }
}

void BoilerCalculate::setWaterHeatDays(const QString &text)
{
    setHeatDays(text, wModeSpin2);
}

void BoilerCalculate::setSteamHeatDays(const QString &text)
{
    setHeatDays(text, sModeSpin2);
}

void BoilerCalculate::setHeatDays(const QString &text,
                                  QSpinBox *scumHeatSpin)
{
    QStringList paths = text.split("/");
    QString mark = paths.at(paths.size()-1);

    QStandardItemModel *model = m_boilerModels->heatModel();
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

    scumHeatSpin->setValue(  items.at(0).data(Qt::UserRole + 1).toInt() );
    emit scumHeatSpin->editingFinished();
}

void BoilerCalculate::initCompleters()
{
    auto completer = new QCompleter(this);
    completer->setModel(m_boilerModels->waterBoilerModel());
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionColumn(0);
    completer->setMaxVisibleItems(50);

    auto treeView = new QTreeView();
    completer->setPopup(treeView);
    treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    wBoilerCombo->setEditable(true);
    wBoilerCombo->setAutoCompletion(true);

    wBoilerCombo->setModel(m_boilerModels->waterBoilerModel());
    wBoilerCombo->setCompleter(completer);

    connect(wBoilerCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(waterBoilerChanged(int)));

    completer = new QCompleter(this);
    completer->setModel(m_boilerModels->steamBoilerModel());
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionColumn(0);
    completer->setMaxVisibleItems(50);

    treeView = new QTreeView();
    completer->setPopup(treeView);
    treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    sBoilerCombo->setEditable(true);
    sBoilerCombo->setAutoCompletion(true);

    sBoilerCombo->setModel(m_boilerModels->steamBoilerModel());
    sBoilerCombo->setCompleter(completer);

    connect(sBoilerCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(steamBoilerChanged(int)));

    completer = new QCompleter(this);
    completer->setModel(m_boilerModels->scumModel());
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionColumn(0);
    completer->setMaxVisibleItems(50);

    wScumTypeCombo->setModel(m_boilerModels->scumModel());
    wScumTypeCombo->setCompleter(completer);
    connect(wScumTypeCombo, SIGNAL(activated(int)), this, SLOT(setWaterScumBorder(int)));

    sScumTypeCombo->setModel(m_boilerModels->scumModel());
    sScumTypeCombo->setCompleter(completer);
    connect(sScumTypeCombo, SIGNAL(activated(int)), this, SLOT(setSteamScumBorder(int)));

    auto treeCompleter = new TreeModelCompleter(this);
    treeCompleter->setModel(m_boilerModels->heatModel());
    treeCompleter->setSeparator(QLatin1String("/"));
    treeCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    treeCompleter->setCompletionMode(QCompleter::PopupCompletion);

    connect(treeCompleter, SIGNAL(activated(QString)), this, SLOT(setWaterHeatDays(QString)));

    wGeoLine->setCompleter(treeCompleter);

    treeCompleter = new TreeModelCompleter(this);
    treeCompleter->setModel(m_boilerModels->heatModel());
    treeCompleter->setSeparator(QLatin1String("/"));
    treeCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    treeCompleter->setCompletionMode(QCompleter::PopupCompletion);

    connect(treeCompleter, SIGNAL(activated(QString)), this, SLOT(setSteamHeatDays(QString)));

    sGeoLine->setCompleter(treeCompleter);
}


void BoilerCalculate::boilerTypeChanged(int index)
{
    boilerType = index;
    stackedWidget->setCurrentIndex(index);
    showPane(1,0);
}

void BoilerCalculate::showPane(int left, int right)
{
    QList<int> list;
    list << 100 * left << 100 * right;

    splitter->setSizes(list);
}

void BoilerCalculate::toFormButton_released()
{
    showPane(1,0);
}

void BoilerCalculate::calculateButton_released()
{
    QList<QLineEdit*> lineEditList;
    lineEditList << RScum <<
                    efficiencyScum50 <<
                    efficiencyScum100 <<
                    loadAverage <<
                    efficiencyScumAgv <<
                    efficiencyAgv <<
                    termalConductAvg <<
                    fuelConsumption <<
                    efficiencyIncrease <<
                    reduceFuel <<
                    reduceFuelAnnual <<
                    annualSavings <<
                    paybackPreiod;

    for( int i = 0; i < lineEditList.size(); i++) {
        lineEditList.at(i)->clear();
    }

    detailEditor->clear();

    /*
    wBoilerCombo
    + wPowerSpin1
    + wPowerSpin2
    + wModeSpin1
    + wModeSpin2
    wFuelCombo
    + wFuelCostSpin
    + wFuelHeatSpin
    + wScumThickSpin
    wScumTypeCombo
    + wScumHeatSpin

    sBoilerCombo
    + sPowerSpin1
    + sPowerSpin2
    + sModeSpin1
    + sModeSpin2
    sFuelCombo
    + sFuelCostSpin
    + sFuelHeatSpin
    + sScumThickSpin
    sScumTypeCombo
    + sScumHeatSpin
    sInputWaterTemp
    */

//    QComboBox *boilerCombo = boilerType == 0 ? wBoilerCombo : sBoilerCombo;
//    QComboBox *fuelCombo = boilerType == 0 ? wFuelCombo : sFuelCombo;

    QDoubleSpinBox *fuelHeatSpin = boilerType == 0 ? wFuelHeatSpin : sFuelHeatSpin;
    QDoubleSpinBox *fuelCostSpin = boilerType == 0 ? wFuelCostSpin : sFuelCostSpin;
    QDoubleSpinBox *scumThickSpin = boilerType == 0 ? wScumThickSpin : sScumThickSpin;
    QDoubleSpinBox *scumHeatSpin = boilerType == 0 ? wScumHeatSpin : sScumHeatSpin;

    QSpinBox *powerSpin2 = boilerType == 0 ? wPowerSpin2 : sPowerSpin2;
    QSpinBox *modeSpin1 = boilerType == 0 ? wModeSpin1 : sModeSpin1;
    QSpinBox *modeSpin2 = boilerType == 0 ? wModeSpin2 : sModeSpin2;

    qreal fuelHeat = fuelHeatSpin->value(); // Qch
    qreal fuelCost = fuelCostSpin->value(); // Ct
    qreal scumThick = scumThickSpin->value(); // SigmaScum
    qreal scumHeat = scumHeatSpin->value(); // Lscum
    qreal wPower = boilerType == 0 ? wPowerSpin1->value() : 0; // Qcp
    qreal sPower = boilerType == 1 ? sPowerSpin1->value() : 0; // Dnp
    qreal powerProc = powerSpin2->value(); // qcp
    qreal modeN = modeSpin1->value(); // ncp
    qreal modeD = modeSpin2->value(); // D

    qreal inputWaterTemp = sInputWaterTemp->value(); // tpv


    bool isRequiredEmpty = false;

    fuelHeatSpin->setStyleSheet("border: 1px solid " + QString(fuelHeat == 0 ? "red" : "gray"));
    fuelCostSpin->setStyleSheet("border: 1px solid " + QString(fuelCost == 0 ? "red" : "gray"));

    scumThickSpin->setStyleSheet("border: 1px solid " + QString(scumThick == 0 ? "red" : "gray"));
    scumHeatSpin->setStyleSheet("border: 1px solid " + QString(scumHeat == 0 ? "red" : "gray"));

    if ( boilerType == 0 ) {
        wPowerSpin1->setStyleSheet("border: 1px solid " + QString(powerProc == 0 && wPower == 0 ? "red" : "gray"));
        powerSpin2->setStyleSheet("border: 1px solid " + QString(powerProc == 0 && wPower == 0 ? "red" : "gray"));
    } else {
        // boilerType == 1
        sPowerSpin1->setStyleSheet("border: 1px solid " + QString(powerProc == 0 && sPower == 0 ? "red" : "gray"));
        powerSpin2->setStyleSheet("border: 1px solid " + QString(powerProc == 0 && sPower == 0 ? "red" : "gray"));
    }

    modeSpin1->setStyleSheet("border: 1px solid " + QString(modeN == 0 && modeD == 0 ? "red" : "gray"));
    modeSpin2->setStyleSheet("border: 1px solid " + QString(modeN == 0 && modeD == 0 ? "red" : "gray"));

    if (fuelHeat == 0) isRequiredEmpty = true;
    if (fuelCost == 0) isRequiredEmpty = true;
    if (scumThick == 0) isRequiredEmpty = true;
    if (scumHeat == 0 ) isRequiredEmpty = true;
    if (boilerType == 0 ) {
        if ( powerProc == 0 && wPower == 0 ) isRequiredEmpty = true;
    } else {
        // boilerType == 1
        if ( inputWaterTemp == 0 ) isRequiredEmpty = true;
        if ( powerProc == 0 && sPower == 0 ) isRequiredEmpty = true;
    }
    if ( modeN == 0 && modeD == 0 ) isRequiredEmpty = true;

    if ( isRequiredEmpty )
        return;

/*
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData( boilerCombo->currentText().toLatin1() );
    hash.addData( fuelCombo->currentText().toLatin1() );

    hash.addData( QString::number(fuelHeat).toLatin1() );
    hash.addData( QString::number(fuelCost).toLatin1() );
    hash.addData( QString::number(scumThick).toLatin1() );
    hash.addData( QString::number(scumHeat).toLatin1() );
    hash.addData( QString::number(boilerType).toLatin1() );
    hash.addData( QString::number(powerProc).toLatin1() );
    hash.addData( QString::number(boilerType == 0 ? wPower : sPower).toLatin1() );
    hash.addData( QString::number(modeN).toLatin1() );
    hash.addData( QString::number(modeD).toLatin1() );
    if (boilerType == 1 ) hash.addData( QString::number(inputWaterTemp).toLatin1() );

    qDebug() <<  hash.result().toHex();

    qreal ΔВТ = 0.0;
    qreal Rнак = 0.0;
*/

    qreal k0, k1, k2, k3, k4, k5;
    qreal R_Scum = 0.0,
            kpd_100_scum = 0.0,
            kpd_50_scum = 0.0,
            kpd_scum = 0.0,
            kpd = 0.0,
            fuelRate = 0.0,
            deltaKpd = 0.0,
            delta_fuel_rate = 0.0,
            annual_delta_fuel_rate = 0.0,
            annual_savings = 0.0;

    detailEditor->append("<b>Техническая часть</b><br/>");

    detailEditor->append("<br/>1. Термическое сопротивление накипи<br/>");

    k0 = 0.001;
    k1 = 0.03;

    R_Scum = (scumThick * k0) / scumHeat;

    RScum->setText( QString::number(R_Scum) );

    // 1. Термическое сопротивление накипи

    detailEditor->append(
        "<b>R<sub>нак</sub></b> = ( " + QString(QChar(0x03B4)) + "<sub>нак</sub> * " + QString::number(k0) + " ) / " +
                QString(QChar(0x03BB)) + "<sub>нак</sub>"
        + "<br/>" +
        "<b>R<sub>нак</sub></b> = ( " + QString::number(scumThick) + " * " + QString::number(k0) + " ) / "
            + QString::number(scumHeat)
            + " = <b>" + QString::number(R_Scum) + "</b>"
        + "<br/>"
    );

    RScum->setStyleSheet("border: 1px solid " + QString(R_Scum > k1 ? "red" : "gray"));


    detailEditor->append("<br/>2. КПД котла с накипью при режиме 50-ти процентной нагрузки с накипью<br/>");

    switch (boilerType) {
    case 0: {
        // КПД котла с накипью при режиме 50% процентной нагрузки с накипью
        k0 = -5 * qPow(10, 6);
        k1 = 592943;
        k2 = 25869;
        k3 = 189.61;
        k4 = 95.211;
        kpd_50_scum = k0 * qPow(R_Scum, 4) + k1 * qPow(R_Scum, 3) - k2 * qPow(R_Scum, 2) - k3 * R_Scum + k4;
        efficiencyScum50->setText( QString::number(kpd_50_scum) );

        detailEditor->append(
            "<b>" + QString(QChar(0x03b7)) + "<sup>50</sup><sub>ка.НАК</sub></b> = "
                    + QString::number(k0) + " * R<sub>нак</sub><sup>4</sup>"
                    + " + " + QString::number(k1) + " * R<sub>нак</sub><sup>3</sup>"
                    + " - " + QString::number(k2) + " * R<sub>нак</sub><sup>2</sup>"
                    + " - " + QString::number(k3) + " * R<sub>нак</sub>"
                    + " + " + QString::number(k4)
            + "<br/>" +
            "<b>" + QString(QChar(0x03b7)) + "<sup>50</sup><sub>ка.НАК</sub></b> = "
                    + QString::number(k0) + " * " + QString::number( qPow(R_Scum, 4) )
                    + " + " + QString::number(k1) + " * " + QString::number( qPow(R_Scum, 3) )
                    + " - " + QString::number(k2) + " * " + QString::number( qPow(R_Scum, 2) )
                    + " - " + QString::number(k3) + " * " + QString::number( R_Scum )
                    + " + " + QString::number( k4 )  +
            " = <b>" +  QString::number(kpd_50_scum) + "</b>"
            + "<br/>"
        );
        break;
    }
    case 1:
        k0 = 22853;
        k1 = 1854.9;
        k2 = 89.607;
        kpd_50_scum = k0 * qPow(R_Scum, 2) - k1 * R_Scum + k2;

        efficiencyScum50->setText( QString::number(kpd_50_scum) );

        detailEditor->append(
            "<b>" + QString(QChar(0x03b7)) + "<sup>50</sup><sub>ка.НАК</sub></b> = "
                    + QString::number(k0) + " * R<sub>нак</sub><sup>2</sup>"
                    + " - " + QString::number(k1) + " * R<sub>нак</sub>"
                    + " + " + QString::number(k2)
            + "<br/>" +
            "<b>" + QString(QChar(0x03b7)) + "<sup>50</sup><sub>ка.НАК</sub></b> = "
                    + QString::number(k0) + " * " + QString::number( qPow(R_Scum, 2) )
                    + " - " + QString::number(k1) + " * " + QString::number( R_Scum )
                    + " + " + QString::number(k2)  +
            " = <b>" +  QString::number(kpd_50_scum) + "</b>"
            + "<br/>"
        );
        break;
    }

    detailEditor->append("<br/>3. КПД котла с накипью при режиме 100 процентной нагрузки с накипью<br/>");

    k0 = boilerType == 0 ? 7187.8 : 42466;
    k1 = boilerType == 0 ? 1434.2 : 2513.1;
    k2 = boilerType == 0 ? 94.515 : 89.405;

    kpd_100_scum = k0 * qPow(R_Scum, 2) - k1 * R_Scum + k2;
    efficiencyScum100->setText( QString::number(kpd_100_scum) );

    detailEditor->append(
        "<b>" + QString(QChar(0x03b7)) + "<sup>100</sup><sub>ка.НАК</sub></b> = "
                + QString::number(k0) + " * R<sub>нак</sub><sup>2</sup>"
                + " - " + QString::number(k1) + " * R<sub>нак</sub>"
                + " + " + QString::number(k2)
        + "<br/>" +
        "<b>" + QString(QChar(0x03b7)) + "<sup>100</sup><sub>ка.НАК</sub></b> = "
                + QString::number(k0) + " * " + QString::number( qPow(R_Scum, 2) )
                + " - " + QString::number(k1) + " * " + QString::number( R_Scum )
                + " + " + QString::number(k2)
                + " = <b>" +  QString::number(kpd_100_scum) + "</b>"
        + "<br/>"
    );


    detailEditor->append("<br/>4. Нагрузка в среднем режиме в процентах от максимальной<br/>");
    if ( powerProc == 0 ) {
        k0 = boilerType == 0 ? 4.65 : 10;
        k1 = boilerType == 0 ? wPower : sPower;
        k2 = 100;

        powerProc = (k1 / k0) * k2;

        loadAverage->setText( QString::number(powerProc) );

        detailEditor->append(
            "<b>q<sub>СР</sub></b> = (Q<sub>ср</sub> / " + QString::number(k0) + ") * 100"
             + "<br/>" +
             "<b>q<sub>СР</sub></b> = (" + QString::number(k1) + " / " + QString::number(k0) + ") * " + QString::number(k2) + " = <b>" + QString::number(powerProc) + "</b>"
            + "<br/>"
        );
    } else {
        loadAverage->setText( QString::number(powerProc) );
        detailEditor->append( "q<sub>СР</sub> = " + QString::number(powerProc) + "<br/>" );
    }

    // 5. КПД котла в среднем режиме с накипью
    detailEditor->append("<br/>5. КПД котла в среднем режиме с накипью<br/>");
    k0 = 50;
    kpd_scum = ((kpd_100_scum - kpd_50_scum) / k0) * (powerProc - k0) + kpd_50_scum;

    efficiencyScumAgv->setText(QString::number(kpd_scum));
    detailEditor->append(
        "<b>" + QString(QChar(0x03b7)) + "<sup>ср</sup><sub>ка.НАК</sub></b> = "
            + "((" + QString(QChar(0x03b7)) + "<sup>100</sup><sub>ка.НАК</sub> " + " - " + QString(QChar(0x03b7)) + "<sup>50</sup><sub>ка.НАК</sub>) / " +QString::number(k0) + " ) "
            + " * (q<sub>СР</sub> - " + QString::number(k0) + ") + " + QString(QChar(0x03b7)) + "<sup>50</sup><sub>ка.НАК</sub>"
        + "<br/>" +
        "<b>" + QString(QChar(0x03b7)) + "<sup>ср</sup><sub>ка.НАК</sub></b> = "
            + "((" + QString::number(kpd_100_scum) + " - " + QString::number(kpd_50_scum) + ") / " + QString::number(k0) + " ) "
            + " * (" + QString::number(powerProc) + " - " + QString::number(k0) + ") + " + QString::number(kpd_50_scum)
            + " = <b>" + QString::number(kpd_scum) + "</b>"
        + "<br/>"
    );

    // 6. КПД котла в среднем режиме без накипи
    detailEditor->append("<br/>6. КПД котла в среднем режиме без накипи<br/>");
    k1 = boilerType == 0 ? 94.515 : 89.405;
    k2 = boilerType == 0 ? 95.211 : 89.607;
    kpd = (( k1 - k2 ) / k0) * (powerProc - k0) + k2;
    efficiencyAgv->setText(QString::number(kpd));

    detailEditor->append(
        "<b>" + QString(QChar(0x03b7)) + "<sup>ср</sup><sub>ка.</sub></b> = "
            + "([" + QString::number(k1) + " - " + QString::number(k2) + "] / 50 ) "
            + " * (q<sub>СР</sub> - " + QString::number(k0) + ") + " + QString::number(k2)
        + "<br/>" +
        "<b>" + QString(QChar(0x03b7)) + "<sup>ср</sup><sub>ка.</sub></b> = "
            + "([" + QString::number(k1) + " - " + QString::number(k2) + "] / " +QString::number(k0) + " ) "
            + " * " + QString::number(powerProc) + " - " + QString::number(k0) + ") + " + QString::number(k2)
            + " = <b>" + QString::number(kpd) + "</b>"
        + "<br/>"
    );


    // 7. Теплопроизвдительность в среднем режиме
    detailEditor->append("<br/>7. Теплопроизвдительность в среднем режиме<br/>");
    if (boilerType == 0) {
        if (wPower == 0 ) {
            k0 = boilerType == 0 ? 4.65 : 10;
            k1 = 100;
            wPower = (powerProc * k0 )/ k1;
            termalConductAvg->setText( QString::number(wPower) );
            detailEditor->append(
                        "<b>Q<sub>ср</sub></b> = ( q<sub>СР</sub> * "
                        + QString::number(k0) + ") / " + QString::number(k1)
                        + "<br/>" +
                        "<b>Q<sub>ср</sub></b> = (" + QString::number(powerProc) + " * "
                        + QString::number(k0) + ") / " + QString::number(k1)
                        + " = <b>" + QString::number(wPower) + "</b>"
            );
        } else {
            termalConductAvg->setText( QString::number(wPower) );
            detailEditor->append("<b>Q<sub>ср</sub></b> = <b>" + QString::number(wPower) + "</b><br/>" );
        }
    } else {
        // boilerType = 1
        k0 = 0.278;
        k1 = 2789;
        k2 = 4.19;
        k3 = 0.03;
        k4 = 830;
        k5 = 0.001;
        wPower = (sPower * k0 * (k1 - (inputWaterTemp * k2) ) + k3 * sPower * k0 * (k4 - inputWaterTemp * k2) ) * k5;
        termalConductAvg->setText( QString::number(wPower) );
        detailEditor->append(
            "<b>Q<sub>ср</sub></b> = [ D<sub>нп</sub> * " + QString::number(k0)
                + " * ( " + QString::number(k1) + " - t<sub>пв</sub> * "
                + QString::number(k2) + " ) + " + QString::number(k3)
                + " * D<sub>нп</sub> * " + QString::number(k0)
                + " * ( " + QString::number(k4) + " - t<sub>пв</sub> * "
                + QString::number(k2) + " ) ] * "+ QString::number(k5)
            + "<br/>" +
            "<b>Q<sub>ср</sub></b> = [ " + QString::number(sPower) + " * "
                + QString::number(k0) + " * ( " + QString::number(k1) + " - "
                + QString::number(inputWaterTemp) + " * " + QString::number(k2) + " ) + "
                + QString::number(k3) + " * " + QString::number(sPower) + " * "
                + QString::number(k0) + " * ( " + QString::number(k4) + " - "
                + QString::number(inputWaterTemp) + " * " + QString::number(k2) + " ) ] * "
                + QString::number(k5)
                + " = " + QString::number(wPower)
            + "<br/>"
        );
    }

    // 8. Расход топлива в котле без накипи
    detailEditor->append("<br/>8. Расход топлива в котле без накипи<br/>");
    k0 = 100;
    fuelRate = (wPower / (fuelHeat * kpd) ) * k0;
    fuelConsumption->setText(QString::number(fuelRate));
    detailEditor->append(
        "<b>В<sub>Т</sub></b> = (Q<sub>ср</sub> / ( Q<sup>с</sup><sub>н</sub> * " +  QString(QChar(0x03b7)) + "<sup>ср</sup><sub>ка.</sub> ) ) * " + QString::number(k0)
        + "<br/>" +
        "<b>В<sub>Т</sub></b> = (" + QString::number(wPower) + " / ( " + QString::number(fuelHeat) + " * " + QString::number(kpd) + " ) ) * " + QString::number(k0)
            + " = <b>" + QString::number(fuelRate) + "</b>"
        + "<br/>"
    );

    // 9. Увеличение КПД котла после очистки
    detailEditor->append("<br/>9. Увеличение КПД котла после очистки<br/>");
    deltaKpd = kpd - kpd_scum;
    efficiencyIncrease->setText(QString::number(deltaKpd));
    detailEditor->append(
        "<b>" + QString(QChar(0x03b7)) + "<sub>ка.</sub></b> = " + QString(QChar(0x03b7)) + "<sup>ср</sup><sub>ка.</sub> - " + QString(QChar(0x03b7)) + "<sup>ср</sup><sub>ка.НАК</sub>"
        + "<br/>" +
        "<b>" + QString(QChar(0x03b7)) + "<sub>ка.</sub></b> = " + QString::number(kpd) + " - " + QString::number(kpd_scum)
            + " = <b>" + QString::number(deltaKpd) + "</b>"
        + "<br/>"
    );

    // Экономическая часть
    detailEditor->append("<br/><b>Экономическая часть</b><br/>");

    // 1. Уменьшение расхода топлива после очистки накипи
    detailEditor->append("<br/>1. Уменьшение расхода топлива после очистки накипи<br/>");
    k0 = 100;
    delta_fuel_rate = ( ( wPower * k0 ) / (fuelHeat * kpd_scum ) ) - fuelRate;
    reduceFuel->setText(QString::number(delta_fuel_rate));
    detailEditor->append(
        "<b>" + QString(QChar(0x0394)) + "В<sub>Т</sub></b> = ( ( Q<sub>ср</sub> * " + QString::number(k0) + " ) / ( Q<sup>с</sup><sub>н</sub> * " +  QString(QChar(0x03b7))  + "<sup>ср</sup><sub>ка.НАК</sub> ) ) - В<sub>Т</sub>"
        + "<br/>" +
        "<b>" + QString(QChar(0x0394)) + "В<sub>Т</sub></b> = ( (" + QString::number(wPower) + " * " + QString::number(k0) + ") / ( " + QString::number(fuelHeat) + " * " + QString::number(kpd_scum) + " ) ) - " + QString::number(fuelRate)
            + " = <b>" + QString::number(delta_fuel_rate) + "</b>"
        + "<br/>"
    );

    // 2. Годовое уменьшение расхода топлива после очистки накипи
    detailEditor->append("<br/>2. Годовое уменьшение расхода топлива после очистки накипи<br/>");
    k0 = 3600;

    if (modeN == 0)
        modeN = modeD * 24;

    annual_delta_fuel_rate = delta_fuel_rate * k0 * modeN;
    reduceFuelAnnual->setText(QString("%L1").arg(annual_delta_fuel_rate,-1,'f',0,' '));
    detailEditor->append(
        "<b>" + QString(QChar(0x0394)) + "В<sub>Т.ГОД</sub></b> = " + QString(QChar(0x0394)) + "В<sub>Т</sub> * " + QString::number(k0) + " * n<sub>ср</sub>"
        + "<br/>" +
        "<b>" + QString(QChar(0x0394)) + "В<sub>Т.ГОД</sub></b> = " + QString::number(delta_fuel_rate) + " * " + QString::number(k0) + " * " + QString::number(modeN)
            + " = <b>" + QString::number(annual_delta_fuel_rate) + "</b>"
        + "<br/>"
    );

    // 3. Годовая экономия
    detailEditor->append("<br/>3. Годовая экономия<br/>");
    k0 = 0.001;
    annual_savings = annual_delta_fuel_rate * fuelCost * k0;
    annualSavings->setText(QString("%L1").arg(annual_savings,-1,'f',0,' '));
    detailEditor->append(
        "<b>Э<sub>ГОД</sub></b> = " + QString(QChar(0x0394)) + "В<sub>Т.ГОД</sub> * Ц<sub>Т</sub> * " + QString::number(k0)
        + "<br/>" +
        "<b>Э<sub>ГОД</sub></b> = " + QString::number(annual_delta_fuel_rate) + " * " +  QString::number(fuelCost) +  " * " + QString::number(k0)
            + " = <b>" + QString::number(annual_savings) + "</b>"
        + "<br/>"
    );

    // 4. Срок окупаемости очистки
    detailEditor->append("<br/>4. Срок окупаемости очистки<br/>");
    paybackPreiod->setText("?");
    detailEditor->append(
        QString("<b>Т<sub>ОК</sub></b> = Э<sub>ГОД</sub> / З<sub>ОЧ</sub><br/>") +
        "<b>Т<sub>ОК</sub></b> = " + QString::number(annual_savings) + " / ? = <b>?</b>"
    );

    showPane(0,1);
}

BoilerCalculate::~BoilerCalculate()
{
    delete m_boilerModels;
}

}
