#include "heatvolume.h"

#include <utils/styledbar.h>

#include <QLabel>
#include <QVBoxLayout>

using namespace Utils;

namespace Calculate {

HeatVolume::HeatVolume(QWidget *parent) : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);

    auto styledBar = new StyledBar(); // The black blob on top of the side bar
    styledBar->setObjectName("HeatExchengerBar");

    auto selectorView = new QWidget();
    selectorView->setObjectName("HeatExchengerSelector");
    selectorView->setWindowTitle(tr("Расчет объема системы отопления"));
    selectorView->setAutoFillBackground(true);

    auto activeLabel = new QLabel(tr("Расчет объема системы отопления"));
    QFont font = activeLabel->font();
    font.setBold(true);
    font.setPointSizeF(font.pointSizeF() * 1.2);
    activeLabel->setFont(font);

    auto innerLayout = new QVBoxLayout;
    innerLayout->setSpacing(10);
    innerLayout->setContentsMargins(14, innerLayout->spacing(), 14, 0);
    innerLayout->addWidget(activeLabel);
    innerLayout->addStretch(1);

    auto selectorLayout = new QVBoxLayout(selectorView);
    selectorLayout->setContentsMargins(0, 0, 0, 0);
    selectorLayout->addWidget(styledBar);
    selectorLayout->addLayout(innerLayout);

    setLayout(selectorLayout);
}

}
