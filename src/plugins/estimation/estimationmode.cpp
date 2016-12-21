#include "estimationmode.h"
#include "estimationicons.h"
#include "estimationconstants.h"

#include <coreplugin/coreconstants.h>
#include <coreplugin/icore.h>
#include <coreplugin/modemanager.h>

#include <extensionsystem/pluginmanager.h>

#include <utils/algorithm.h>
#include <utils/icon.h>
#include <utils/fileutils.h>
#include <utils/hostosinfo.h>
#include <utils/styledbar.h>

#include <utils/theme/theme.h>

#include <QVBoxLayout>
#include <QMessageBox>

#include <QDir>

using namespace Estimation;
using namespace Estimation::Constants;
using namespace Estimation::Internal;
using namespace Estimation::Icons;

using namespace Core;
using namespace Utils;
using namespace ExtensionSystem;

// static const char currentPageSettingsKeyC[] = "EstimationTab";

EstimationMode::EstimationMode()
{
    setDisplayName(tr("Estimation"));

    setIcon(Utils::Icon::modeIcon(MODE_ESTIMATION_CLASSIC,
                                  MODE_ESTIMATION_FLAT,
                                  MODE_ESTIMATION_FLAT_ACTIVE));

    setPriority(Constants::P_MODE_ESTIMATION);
    setId(Constants::MODE_ESTIMATION);
    setContext(Context(Constants::C_ESTIMATION_MODE));

    m_modeWidget = new QWidget;
    m_modeWidget->setObjectName(QLatin1String("EstimationModeWidget"));
    QVBoxLayout *layout = new QVBoxLayout(m_modeWidget);
    layout->setMargin(0);
    layout->setSpacing(0);

    m_matchPage = new QWidget(m_modeWidget);
    m_matchPage->setObjectName(QLatin1String("Athlete"));

    StyledBar *styledBar = new StyledBar(m_modeWidget);
    styledBar->setObjectName(QLatin1String("AthleteStyledBar"));
    layout->addWidget(styledBar);

    m_modeWidget->setLayout(layout);
    layout->addWidget(m_matchPage);

    setWidget(m_modeWidget);
}

EstimationMode::~EstimationMode()
{
    delete m_modeWidget;
}


