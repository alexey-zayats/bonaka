
#include "plugindialog.h"

#include <extensionsystem/pluginmanager.h>
#include <extensionsystem/pluginspec.h>

#include <widgets/pluginview.h>
#include <widgets/plugindetailsview.h>
#include <widgets/pluginerrorview.h>

#include <utils/lineedit.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

namespace Core {
namespace Internal {

static bool s_isRestartRequired = false;

PluginDialog::PluginDialog(QWidget *parent)
    : QDialog(parent),
      m_view(new Widgets::PluginView(this))
{
    QVBoxLayout *vl = new QVBoxLayout(this);

    auto filterEdit = new Utils::LineEdit(this);
    filterEdit->setFiltering(true);
    connect(filterEdit, &Utils::LineEdit::filterChanged,
            m_view, &Widgets::PluginView::setFilter);
    vl->addWidget(filterEdit);

    vl->addWidget(m_view);

    m_detailsButton = new QPushButton(tr("Details"), this);
    m_errorDetailsButton = new QPushButton(tr("Error Details"), this);
    m_closeButton = new QPushButton(tr("Close"), this);
    m_detailsButton->setEnabled(false);
    m_errorDetailsButton->setEnabled(false);
    m_closeButton->setEnabled(true);
    m_closeButton->setDefault(true);

    m_restartRequired = new QLabel(tr("Restart required."), this);
    if (!s_isRestartRequired)
        m_restartRequired->setVisible(false);

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addWidget(m_detailsButton);
    hl->addWidget(m_errorDetailsButton);
    hl->addSpacing(10);
    hl->addWidget(m_restartRequired);
    hl->addStretch(5);
    hl->addWidget(m_closeButton);

    vl->addLayout(hl);

    resize(650, 400);
    setWindowTitle(tr("Installed Plugins"));

    connect(m_view, &Widgets::PluginView::currentPluginChanged,
            this, &PluginDialog::updateButtons);
    connect(m_view, &Widgets::PluginView::pluginActivated,
            this, &PluginDialog::openDetails);
    connect(m_view, &Widgets::PluginView::pluginSettingsChanged,
            this, &PluginDialog::updateRestartRequired);
    connect(m_detailsButton, &QAbstractButton::clicked,
            [this]  { openDetails(m_view->currentPlugin()); });
    connect(m_errorDetailsButton, &QAbstractButton::clicked,
            this, &PluginDialog::openErrorDetails);
    connect(m_closeButton, &QAbstractButton::clicked,
            this, &PluginDialog::closeDialog);
    updateButtons();
}

void PluginDialog::closeDialog()
{
    ExtensionSystem::PluginManager::writeSettings();
    accept();
}

void PluginDialog::updateRestartRequired()
{
    // just display the notice all the time after once changing something
    s_isRestartRequired = true;
    m_restartRequired->setVisible(true);
}

void PluginDialog::updateButtons()
{
    ExtensionSystem::PluginSpec *selectedSpec = m_view->currentPlugin();
    if (selectedSpec) {
        m_detailsButton->setEnabled(true);
        m_errorDetailsButton->setEnabled(selectedSpec->hasError());
    } else {
        m_detailsButton->setEnabled(false);
        m_errorDetailsButton->setEnabled(false);
    }
}

void PluginDialog::openDetails(ExtensionSystem::PluginSpec *spec)
{
    if (!spec)
        return;
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Plugin Details of %1").arg(spec->name()));
    QVBoxLayout *layout = new QVBoxLayout;
    dialog.setLayout(layout);
    Widgets::PluginDetailsView *details = new Widgets::PluginDetailsView(&dialog);
    layout->addWidget(details);
    details->update(spec);
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close, Qt::Horizontal, &dialog);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog.resize(400, 500);
    dialog.exec();
}

void PluginDialog::openErrorDetails()
{
    ExtensionSystem::PluginSpec *spec = m_view->currentPlugin();
    if (!spec)
        return;
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Plugin Errors of %1").arg(spec->name()));
    QVBoxLayout *layout = new QVBoxLayout;
    dialog.setLayout(layout);
    Widgets::PluginErrorView *errors = new Widgets::PluginErrorView(&dialog);
    layout->addWidget(errors);
    errors->update(spec);
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close, Qt::Horizontal, &dialog);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog.resize(500, 300);
    dialog.exec();
}

} // namespace Internal
} // namespace Core
