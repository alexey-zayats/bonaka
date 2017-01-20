#include "entity_mode.h"
#include "entity_icons.h"
#include "entity_constants.h"

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

using namespace Entity;
using namespace Entity::Constants;
using namespace Entity::Internal;
using namespace Entity::Icons;

using namespace Core;
using namespace Utils;
using namespace ExtensionSystem;

// static const char currentPageSettingsKeyC[] = "EstimationTab";

EntityMode::EntityMode() :
    m_modeWidget (0),
    m_innerPage (0)
{
    setDisplayName(tr("Entity"));

    setIcon(Utils::Icon::modeIcon(MODE_ENTITY_CLASSIC,
                                  MODE_ENTITY_FLAT,
                                  MODE_ENTITY_FLAT_ACTIVE));

    setPriority(Constants::P_MODE_ENTITY);
    setId(Constants::MODE_ENTITY);
    setContext(Context(Constants::C_ENTITY_MODE));

    m_modeWidget = new QWidget;
    m_modeWidget->setObjectName(QLatin1String("EntityModeWidget"));
    QVBoxLayout *layout = new QVBoxLayout(m_modeWidget);
    layout->setMargin(0);
    layout->setSpacing(0);

    m_innerPage = new QWidget(m_modeWidget);
    m_innerPage->setObjectName(QLatin1String("Entitys"));

    StyledBar *styledBar = new StyledBar(m_modeWidget);
    styledBar->setObjectName(QLatin1String("EntityStyledBar"));
    layout->addWidget(styledBar);

    m_modeWidget->setLayout(layout);
    layout->addWidget(m_innerPage);

    setWidget(m_modeWidget);
}

EntityMode::~EntityMode()
{
    delete m_innerPage;
    delete m_modeWidget;
}


