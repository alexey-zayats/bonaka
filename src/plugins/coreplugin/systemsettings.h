
#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H

#include <coreplugin/dialogs/ioptionspage.h>
#include <QPointer>

QT_BEGIN_NAMESPACE
class QMessageBox;
QT_END_NAMESPACE

namespace Core {
namespace Internal {

namespace Ui { class SystemSettings; }

class SystemSettings : public IOptionsPage
{
    Q_OBJECT

public:
    SystemSettings();

    QWidget *widget();
    void apply();
    void finish();

private:
    Ui::SystemSettings *m_page;
    QPointer<QMessageBox> m_dialog;
    QPointer<QWidget> m_widget;
};

} // namespace Internal
} // namespace Core

#endif // SYSTEMSETTINGS_H
