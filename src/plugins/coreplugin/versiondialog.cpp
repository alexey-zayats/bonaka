
#include "versiondialog.h"

#include <bonaka/bonaka_version.h>
#include <coreplugin/coreicons.h>
#include <coreplugin/icore.h>
#include <coreplugin/coreconstants.h>
#include <utils/algorithm.h>
#include <utils/hostosinfo.h>

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QDate>

using namespace Core;
using namespace Core::Internal;

VersionDialog::VersionDialog(QWidget *parent)
    : QDialog(parent)
{
    // We need to set the window icon explicitly here since for some reason the
    // application icon isn't used when the size of the dialog is fixed (at least not on X11/GNOME)
    if (Utils::HostOsInfo::isLinuxHost())
        setWindowIcon(Icons::LOGO_128.icon());

    setWindowTitle(tr("About Bonaka"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QGridLayout *layout = new QGridLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    QString ideRev;
#ifdef APP_REVISION
     ideRev = tr("From revision %1<br/>").arg(QString::fromLatin1(Constants::APP_REVISION_STR));
#endif
     QString buildDateInfo;

#ifdef APP_SHOW_BUILD_DATE
     buildDateInfo = tr("Built on %1 %2<br/>").arg(QLatin1String(__DATE__), QLatin1String(__TIME__));
#endif

    const QString br = QLatin1String("<br/>");
    const QStringList additionalInfoLines = ICore::additionalAboutInformation();
    const QString additionalInfo =
            QStringList(Utils::transform(additionalInfoLines, &QString::toHtmlEscaped)).join(br);


    const QString description = tr(
        "<h3>%1</h3>"
        "%2<br/>"
        "%3"
        "%4"
        "%5"
        "<br/>"
        "&copy; %6 %7. All rights reserved.<br/>"
        "<br/>"
        )
        .arg(ICore::versionString(),
             ICore::buildCompatibilityString(),
             buildDateInfo,
             ideRev,
             additionalInfo.isEmpty() ? QString() : br + additionalInfo + br,
             (
                 QString::compare (QDate::currentDate().toString(QLatin1String("yyyy")),
                                   QLatin1String(Constants::FOUNDED_AT)) == 0 ?
                     QLatin1String(Constants::APP_YEAR) :
                     QString( QLatin1String("%1-%2") ).arg(QLatin1String(Constants::FOUNDED_AT)).arg( QLatin1String(Constants::APP_YEAR) )
             ),
             QLatin1String(Constants::APP_AUTHOR));

    QLabel *copyRightLabel = new QLabel(description);
    copyRightLabel->setWordWrap(true);
    copyRightLabel->setOpenExternalLinks(true);
    copyRightLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    QPushButton *closeButton = buttonBox->button(QDialogButtonBox::Close);
    buttonBox->addButton(closeButton, QDialogButtonBox::ButtonRole(QDialogButtonBox::RejectRole | QDialogButtonBox::AcceptRole));
    connect(buttonBox , &QDialogButtonBox::rejected, this, &QDialog::reject);

    QLabel *logoLabel = new QLabel;
    logoLabel->setPixmap(Icons::LOGO_128.pixmap());
    layout->addWidget(logoLabel , 0, 0, 1, 1);
    layout->addWidget(copyRightLabel, 0, 1, 4, 4);
    layout->addWidget(buttonBox, 4, 0, 1, 5);
}

bool VersionDialog::event(QEvent *event)
{
    if (event->type() == QEvent::ShortcutOverride) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Escape && !ke->modifiers()) {
            ke->accept();
            return true;
        }
    }
    return QDialog::event(event);
}
