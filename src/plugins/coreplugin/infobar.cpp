#include "infobar.h"

#include "coreicons.h"
#include "icore.h"

#include <utils/theme/theme.h>

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

static const char C_SUPPRESSED_WARNINGS[] = "SuppressedWarnings";

using namespace Utils;

namespace Core {

QSet<Id> InfoBar::globallySuppressed;

InfoBarEntry::InfoBarEntry(Id _id, const QString &_infoText, GlobalSuppressionMode _globalSuppression)
    : id(_id)
    , infoText(_infoText)
    , globalSuppression(_globalSuppression)
{
}

void InfoBarEntry::setCustomButtonInfo(const QString &_buttonText, CallBack callBack)
{
    buttonText = _buttonText;
    m_buttonCallBack = callBack;
}

void InfoBarEntry::setCancelButtonInfo(CallBack callBack)
{
    m_cancelButtonCallBack = callBack;
}

void InfoBarEntry::setCancelButtonInfo(const QString &_cancelButtonText, CallBack callBack)
{
    cancelButtonText = _cancelButtonText;
    m_cancelButtonCallBack = callBack;
}


void InfoBar::addInfo(const InfoBarEntry &info)
{
    m_infoBarEntries << info;
    emit changed();
}

void InfoBar::removeInfo(Id id)
{
    QMutableListIterator<InfoBarEntry> it(m_infoBarEntries);
    while (it.hasNext())
        if (it.next().id == id) {
            it.remove();
            emit changed();
            return;
        }
}

bool InfoBar::containsInfo(Id id) const
{
    QListIterator<InfoBarEntry> it(m_infoBarEntries);
    while (it.hasNext())
        if (it.next().id == id)
            return true;

    return false;
}

// Remove and suppress id
void InfoBar::suppressInfo(Id id)
{
    removeInfo(id);
    m_suppressed << id;
}

// Info can not be added more than once, or if it is suppressed
bool InfoBar::canInfoBeAdded(Id id) const
{
    return !containsInfo(id) && !m_suppressed.contains(id) && !globallySuppressed.contains(id);
}

void InfoBar::enableInfo(Id id)
{
    m_suppressed.remove(id);
}

void InfoBar::clear()
{
    if (!m_infoBarEntries.isEmpty()) {
        m_infoBarEntries.clear();
        emit changed();
    }
}

void InfoBar::globallySuppressInfo(Id id)
{
    globallySuppressed.insert(id);
    QStringList list;
    foreach (Id i, globallySuppressed)
        list << QLatin1String(i.name());
    ICore::settings()->setValue(QLatin1String(C_SUPPRESSED_WARNINGS), list);
}

void InfoBar::initializeGloballySuppressed()
{
    QStringList list = ICore::settings()->value(QLatin1String(C_SUPPRESSED_WARNINGS)).toStringList();
    foreach (const QString &id, list)
        globallySuppressed.insert(Id::fromString(id));
}

void InfoBar::clearGloballySuppressed()
{
    globallySuppressed.clear();
    ICore::settings()->setValue(QLatin1String(C_SUPPRESSED_WARNINGS), QStringList());
}

bool InfoBar::anyGloballySuppressed()
{
    return !globallySuppressed.isEmpty();
}


InfoBarDisplay::InfoBarDisplay(QObject *parent)
    : QObject(parent)
    , m_infoBar(0)
    , m_boxLayout(0)
    , m_boxIndex(0)
{
}

void InfoBarDisplay::setTarget(QBoxLayout *layout, int index)
{
    m_boxLayout = layout;
    m_boxIndex = index;
}

void InfoBarDisplay::setInfoBar(InfoBar *infoBar)
{
    if (m_infoBar == infoBar)
        return;

    if (m_infoBar)
        m_infoBar->disconnect(this);
    m_infoBar = infoBar;
    if (m_infoBar) {
        connect(m_infoBar, &InfoBar::changed, this, &InfoBarDisplay::update);
        connect(m_infoBar, &QObject::destroyed, this, &InfoBarDisplay::infoBarDestroyed);
    }
    update();
}

void InfoBarDisplay::infoBarDestroyed()
{
    m_infoBar = 0;
    // Calling update() here causes a complicated crash on shutdown.
    // So instead we rely on the view now being either destroyed (in which case it
    // will delete the widgets itself) or setInfoBar() being called explicitly.
}

void InfoBarDisplay::update()
{
    foreach (QWidget *widget, m_infoWidgets) {
        widget->disconnect(this); // We want no destroyed() signal now
        delete widget;
    }
    m_infoWidgets.clear();

    if (!m_infoBar)
        return;

    foreach (const InfoBarEntry &info, m_infoBar->m_infoBarEntries) {
        QFrame *infoWidget = new QFrame;

        QPalette pal;
        pal.setColor(QPalette::Window, appTheme()->color(Theme::InfoBarBackground));
        pal.setColor(QPalette::WindowText, appTheme()->color(Theme::InfoBarText));

        infoWidget->setPalette(pal);
        infoWidget->setFrameStyle(QFrame::Panel | QFrame::Raised);
        infoWidget->setLineWidth(1);
        infoWidget->setAutoFillBackground(true);

        QHBoxLayout *hbox = new QHBoxLayout(infoWidget);
        hbox->setMargin(2);

        QLabel *infoWidgetLabel = new QLabel(info.infoText);
        infoWidgetLabel->setWordWrap(true);
        hbox->addWidget(infoWidgetLabel);

        if (!info.buttonText.isEmpty()) {
            QToolButton *infoWidgetButton = new QToolButton;
            infoWidgetButton->setText(info.buttonText);
            connect(infoWidgetButton, &QAbstractButton::clicked, [info]() { info.m_buttonCallBack(); });

            hbox->addWidget(infoWidgetButton);
        }

        const Id id = info.id;
        QToolButton *infoWidgetSuppressButton = 0;
        if (info.globalSuppression == InfoBarEntry::GlobalSuppressionEnabled) {
            infoWidgetSuppressButton = new QToolButton;
            infoWidgetSuppressButton->setText(tr("Do Not Show Again"));
            connect(infoWidgetSuppressButton, &QAbstractButton::clicked, this, [this, id] {
                m_infoBar->removeInfo(id);
                InfoBar::globallySuppressInfo(id);
            });
        }

        QToolButton *infoWidgetCloseButton = new QToolButton;
        // need to connect to cancelObjectbefore connecting to cancelButtonClicked,
        // because the latter removes the button and with it any connect
        if (info.m_cancelButtonCallBack)
            connect(infoWidgetCloseButton, &QAbstractButton::clicked, info.m_cancelButtonCallBack);
        connect(infoWidgetCloseButton, &QAbstractButton::clicked, this, [this, id] {
            m_infoBar->suppressInfo(id);
        });

        if (info.cancelButtonText.isEmpty()) {
            infoWidgetCloseButton->setAutoRaise(true);
            infoWidgetCloseButton->setIcon(Icons::CLOSE_FOREGROUND.icon());
            infoWidgetCloseButton->setToolTip(tr("Close"));
            if (infoWidgetSuppressButton)
                hbox->addWidget(infoWidgetSuppressButton);
            hbox->addWidget(infoWidgetCloseButton);
        } else {
            infoWidgetCloseButton->setText(info.cancelButtonText);
            hbox->addWidget(infoWidgetCloseButton);
            if (infoWidgetSuppressButton)
                hbox->addWidget(infoWidgetSuppressButton);
        }

        connect(infoWidget, &QObject::destroyed, this, &InfoBarDisplay::widgetDestroyed);
        m_boxLayout->insertWidget(m_boxIndex, infoWidget);
        m_infoWidgets << infoWidget;
    }
}

void InfoBarDisplay::widgetDestroyed()
{
    m_infoWidgets.removeOne(static_cast<QWidget *>(sender()));
}

} // namespace Core