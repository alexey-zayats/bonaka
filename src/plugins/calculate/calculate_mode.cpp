#include "calculate_mode.h"
#include "calculate_icons.h"
#include "calculate_constants.h"

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

using namespace Calculate;
using namespace Calculate::Constants;
using namespace Calculate::Internal;
using namespace Calculate::Icons;

using namespace Core;
using namespace Utils;
using namespace ExtensionSystem;

CalculateMode::CalculateMode(QWidget *parent)
{
    setWidget(parent);
    setDisplayName(tr("Расчет"));

    setIcon(Utils::Icon::modeIcon(MODE_CALCULATE_CLASSIC,
                                  MODE_CALCULATE_FLAT,
                                  MODE_CALCULATE_FLAT_ACTIVE));

    setPriority(Constants::P_MODE_CALCULATE);
    setId(Constants::MODE_CALCULATE);
    setContext(Context(Constants::C_CALCULATE_MODE));
}

CalculateMode::~CalculateMode()
{
}


