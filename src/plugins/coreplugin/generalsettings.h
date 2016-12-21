#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include <coreplugin/dialogs/ioptionspage.h>
#include <QPointer>

QT_BEGIN_NAMESPACE
class QMessageBox;
QT_END_NAMESPACE

namespace Core {
namespace Internal {

namespace Ui { class GeneralSettings; }

class GeneralSettings : public IOptionsPage
{
    Q_OBJECT

public:
    GeneralSettings();

    QWidget *widget();
    void apply();
    void finish();

private:
    void resetInterfaceColor();
    void resetWarnings();
    void resetLanguage();

    bool canResetWarnings() const;
    void fillLanguageBox() const;
    QString language() const;
    void setLanguage(const QString&);
    Ui::GeneralSettings *m_page;
    QPointer<QMessageBox> m_dialog;
    QPointer<QWidget> m_widget;
};

} // namespace Internal
} // namespace Core

#endif // GENERALSETTINGS_H
