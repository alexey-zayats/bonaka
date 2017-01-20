#include "registry_mode.h"
#include "registry_icons.h"
#include "registry_constants.h"

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

using namespace Registry;
using namespace Registry::Constants;
using namespace Registry::Internal;
using namespace Registry::Icons;

using namespace Core;
using namespace Utils;
using namespace ExtensionSystem;

// static const char currentPageSettingsKeyC[] = "EstimationTab";

RegistryMode::RegistryMode() :
    m_modeWidget (0),
    m_innerPage (0)
{
    setDisplayName(tr("Registry"));

    setIcon(Utils::Icon::modeIcon(MODE_REGISTRY_CLASSIC,
                                  MODE_REGISTRY_FLAT,
                                  MODE_REGISTRY_FLAT_ACTIVE));

    setPriority(Constants::P_MODE_REGISTRY);
    setId(Constants::MODE_REGISTRY);
    setContext(Context(Constants::C_REGISTRY_MODE));

    m_modeWidget = new QWidget;
    m_modeWidget->setObjectName(QLatin1String("RegistryModeWidget"));
    QVBoxLayout *layout = new QVBoxLayout(m_modeWidget);
    layout->setMargin(0);
    layout->setSpacing(0);

    m_innerPage = new QWidget(m_modeWidget);
    m_innerPage->setObjectName(QLatin1String("Registrys"));

    StyledBar *styledBar = new StyledBar(m_modeWidget);
    styledBar->setObjectName(QLatin1String("RegistryStyledBar"));
    layout->addWidget(styledBar);

    m_modeWidget->setLayout(layout);
    layout->addWidget(m_innerPage);

    setWidget(m_modeWidget);
}

RegistryMode::~RegistryMode()
{
    delete m_innerPage;
    delete m_modeWidget;
}


