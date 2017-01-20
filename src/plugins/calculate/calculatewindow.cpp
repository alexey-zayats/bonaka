#include "calculatewindow.h"

#include "boilerwidget.h"
#include "boilercalculate.h"
#include "heatexchanger.h"
#include "heatvolume.h"

#include <coreplugin/coreicons.h>
#include <coreplugin/icore.h>

#include <extensionsystem/pluginmanager.h>

#include <utils/algorithm.h>
#include <utils/basetreeview.h>
#include <utils/navigationtreeview.h>
#include <utils/styledbar.h>
#include <utils/treemodel.h>
#include <utils/itemviews.h>

#include <QSignalMapper>
#include <QApplication>
#include <QComboBox>
#include <QDockWidget>
#include <QFileDialog>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QTimer>
#include <QTreeView>
#include <QVBoxLayout>

using namespace Core;
using namespace Utils;

namespace Calculate
{

CalculateWindow::CalculateWindow() :
    Utils::MainWindow()
{
    setBackgroundRole(QPalette::Base);

    auto styledBar = new StyledBar; // The black blob on top of the side bar
    styledBar->setObjectName("ProjectModeStyledBar");

    auto selectorView = new QWidget; // Black blob
    selectorView->setObjectName("CalculateSelector");
    selectorView->setWindowTitle(tr("Расчет"));
    selectorView->setAutoFillBackground(true);

    auto activeLabel = new QLabel(tr("Расчет"));
    QFont font = activeLabel->font();
    font.setBold(true);
    font.setPointSizeF(font.pointSizeF() * 1.2);
    activeLabel->setFont(font);

    boilerWidget = new BoilerWidget();
    boilerCalculate = new BoilerCalculate();
    heatExchnagerWidget = new HeatExchanger(this);
    heatVolumeWidget = new HeatVolume(this);

    QSignalMapper *map = new QSignalMapper(this);

    boilerButton = new QPushButton(this);
    boilerButton->setCheckable(true);
//    boilerButton->setChecked(true);
    boilerButton->setAutoExclusive(true);
    boilerButton->setText( tr("Котлы") );

    boilerButton2 = new QPushButton(this);
    boilerButton2->setCheckable(true);
    boilerButton2->setAutoExclusive(true);
    boilerButton2->setText( tr("Котлы2") );

    heatExchangeButton = new QPushButton(this);
    heatExchangeButton->setCheckable(true);
    heatExchangeButton->setAutoExclusive(true);
    heatExchangeButton->setText( tr("Теплообменники") );

    heatVolume = new QPushButton(this);
    heatVolume->setCheckable(true);
    heatVolume->setAutoExclusive(true);
    heatVolume->setText( tr("Объем систем") );

    connect(boilerButton, SIGNAL(toggled(bool)), map, SLOT(map()));
    connect(boilerButton2, SIGNAL(toggled(bool)), map, SLOT(map()));
    connect(heatExchangeButton, SIGNAL(toggled(bool)), map, SLOT(map()));
    connect(heatVolume, SIGNAL(toggled(bool)), map, SLOT(map()));

    map->setMapping(boilerButton, 1);
    map->setMapping(boilerButton2, 2);
    map->setMapping(heatExchangeButton, 3);
    map->setMapping(heatVolume, 4);

    connect(map, SIGNAL(mapped(int)), this, SLOT(widgetChanged(int)));

    calcListWidget = new Utils::ListWidget();

    auto innerLayout = new QVBoxLayout;
    innerLayout->setSpacing(10);
    innerLayout->setContentsMargins(14, innerLayout->spacing(), 14, 0);
    innerLayout->addWidget(activeLabel);
    innerLayout->addWidget(boilerButton);
    innerLayout->addWidget(boilerButton2);
    innerLayout->addWidget(heatExchangeButton);
    innerLayout->addWidget(heatVolume);
    innerLayout->addSpacerItem(new QSpacerItem(10,10));
    innerLayout->addWidget(new QLabel("<b>История</b>"));
    innerLayout->addWidget(calcListWidget);
    innerLayout->addStretch(1);

    auto selectorLayout = new QVBoxLayout(selectorView);
    selectorLayout->setContentsMargins(0, 0, 0, 0);
    selectorLayout->addWidget(styledBar);
    selectorLayout->addLayout(innerLayout);

    auto selectorDock = addDockForWidget(selectorView);
    addDockWidget(Qt::LeftDockWidgetArea, selectorDock);

    // setPanel(boilerWidget);
    boilerButton->setChecked(true);
//    emit boilerButton->toggled(true);
}

void CalculateWindow::widgetChanged(int i)
{
    switch (i) {
    case 1:
        setPanel(boilerWidget);
        break;
    case 2:
        setPanel(boilerCalculate);
        break;
    case 3:
        setPanel(heatExchnagerWidget);
        break;
    case 4:
        setPanel(heatVolumeWidget);
        break;
    }
}

void CalculateWindow::setPanel(QWidget *panel)
{
    if (QWidget *widget = centralWidget()) {
        takeCentralWidget();
        widget->hide(); // Don't delete.
    }
    if (panel) {
        setCentralWidget(panel);
        panel->show();
        if (hasFocus()) // we get assigned focus from setFocusToCurrentMode, pass that on
            panel->setFocus();
    }
}

}
