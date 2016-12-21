#include "estiamtionmode.h"
#include "estimationicons.h"
#include "estimationconstants.h"

#include <coreplugin/coreconstants.h>
#include <coreplugin/icore.h>

#include <coreplugin/modemanager.h>

#include <coreplugin/imatchboard.h>

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

static const char currentPageSettingsKeyC[] = "EstimationTab";

EstimationMode::EstimationMode()
    : m_activePlugin(0)
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
    QSettings *settings = ICore::settings();
    settings->setValue(QLatin1String(currentPageSettingsKeyC), activePlugin());
    delete m_modeWidget;
}

void EstimationMode::initPlugins()
{
    QSettings *settings = ICore::settings();
    setActivePlugin(settings->value(QLatin1String(currentPageSettingsKeyC)).toInt());


    QList<IMatchBoard *> availablePages = PluginManager::getObjects<IMatchBoard>();
    addPages(availablePages);

    // make sure later added pages are made available too:
    connect(PluginManager::instance(), &PluginManager::objectAdded,
            this, &AthleteMode::pluginAdded);
}

void EstimationMode::pluginAdded(QObject *obj)
{
    IMatchBoard *page = qobject_cast<IMatchBoard*>(obj);
    if (!page)
        return;
    addPages(QList<IMatchBoard *>() << page);
}

void EstimationMode::addPages(const QList<IMatchBoard *> &pages)
{
    QList<IMatchBoard *> addedPages = pages;
    Utils::sort(addedPages, [](const IMatchBoard *l, const IMatchBoard *r) {
        return l->priority() < r->priority();
    });

    // insert into m_pluginList, keeping m_pluginList sorted by priority
    auto addIt = addedPages.begin();
    auto currentIt = m_pluginList.begin();
    while (addIt != addedPages.end()) {
        IMatchBoard *page = *addIt;
        while (currentIt != m_pluginList.end() && (*currentIt)->priority() <= page->priority())
            ++currentIt;
        // currentIt is now either end() or a page with higher value
        currentIt = m_pluginList.insert(currentIt, page);
        m_idPageMap.insert(page->id(), page);

//        page->facilitateQml(engine);

        ++currentIt;
        ++addIt;
    }
}
