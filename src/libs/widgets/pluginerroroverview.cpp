#include "pluginerroroverview.h"
#include "ui_pluginerroroverview.h"

#include <extensionsystem/pluginspec.h>
#include <extensionsystem/pluginmanager.h>

Q_DECLARE_METATYPE(ExtensionSystem::PluginSpec*)

using namespace ExtensionSystem;

namespace Widgets
{

PluginErrorOverview::PluginErrorOverview(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui_PluginErrorOverview)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    m_ui->setupUi(this);
    m_ui->buttonBox->addButton(tr("Continue"), QDialogButtonBox::AcceptRole);

    foreach (PluginSpec *spec, PluginManager::plugins()) {
        // only show errors on startup if plugin is enabled.
        if (spec->hasError() && spec->isEffectivelyEnabled()) {
            QListWidgetItem *item = new QListWidgetItem(spec->name());
            item->setData(Qt::UserRole, qVariantFromValue(spec));
            m_ui->pluginList->addItem(item);
        }
    }

    connect(m_ui->pluginList, &QListWidget::currentItemChanged,
            this, &PluginErrorOverview::showDetails);

    if (m_ui->pluginList->count() > 0)
        m_ui->pluginList->setCurrentRow(0);
}

PluginErrorOverview::~PluginErrorOverview()
{
    delete m_ui;
}

void PluginErrorOverview::showDetails(QListWidgetItem *item)
{
    if (item) {
        PluginSpec *spec = item->data(Qt::UserRole).value<PluginSpec *>();
        m_ui->pluginError->setText(spec->errorString());
    } else {
        m_ui->pluginError->clear();
    }
}

} // namespace Widgets
