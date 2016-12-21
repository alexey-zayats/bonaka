
#include "systemsettings.h"
#include "coreconstants.h"
#include "icore.h"

#include <utils/hostosinfo.h>

#include <QCoreApplication>
#include <QMessageBox>
#include <QSettings>

#include "ui_systemsettings.h"

using namespace Utils;

namespace Core {
namespace Internal {

SystemSettings::SystemSettings()
    : m_page(0), m_dialog(0)
{
    setId(Constants::SETTINGS_ID_SYSTEM);
    setDisplayName(tr("System"));
    setCategory(Constants::SETTINGS_CATEGORY_CORE);
    setDisplayCategory(QCoreApplication::translate("Core", Constants::SETTINGS_TR_CATEGORY_CORE));
    setCategoryIcon(QLatin1String(Constants::SETTINGS_CATEGORY_CORE_ICON));
}

QWidget *SystemSettings::widget()
{
    if (!m_widget) {
        m_page = new Ui::SystemSettings();
        m_widget = new QWidget;
        m_page->setupUi(m_widget);
    }
    return m_widget;
}

void SystemSettings::apply()
{
    if (!m_page) // wasn't shown, can't be changed
        return;
}

void SystemSettings::finish()
{
    delete m_widget;
    delete m_page;
    m_page = 0;
}

} // namespace Internal
} // namespace Core
