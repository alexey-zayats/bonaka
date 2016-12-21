#ifndef IOUTPUTPANE_H
#define IOUTPUTPANE_H

#include "core_global.h"

#include <QObject>
#include <QList>
#include <QString>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

namespace Core {

class CORE_EXPORT IOutputPane : public QObject
{
    Q_OBJECT

public:
    IOutputPane(QObject *parent = 0) : QObject(parent) {}

    virtual QWidget *outputWidget(QWidget *parent) = 0;
    virtual QList<QWidget *> toolBarWidgets() const = 0;
    virtual QString displayName() const = 0;

    virtual int priorityInStatusBar() const = 0;

    virtual void clearContents() = 0;
    virtual void visibilityChanged(bool visible) = 0;

    virtual void setFocus() = 0;
    virtual bool hasFocus() const = 0;
    virtual bool canFocus() const = 0;

    virtual bool canNavigate() const = 0;
    virtual bool canNext() const = 0;
    virtual bool canPrevious() const = 0;
    virtual void goToNext() = 0;
    virtual void goToPrev() = 0;

    enum Flag { NoModeSwitch = 0, ModeSwitch = 1, WithFocus = 2, EnsureSizeHint = 4};
    Q_DECLARE_FLAGS(Flags, Flag)

public slots:
    void popup(int flags) { emit showPage(flags); }

    void hide() { emit hidePage(); }
    void toggle(int flags) { emit togglePage(flags); }
    void navigateStateChanged() { emit navigateStateUpdate(); }
    void flash() { emit flashButton(); }
    void setIconBadgeNumber(int number) { emit setBadgeNumber(number); }

signals:
    void showPage(int flags);
    void hidePage();
    void togglePage(int flags);
    void navigateStateUpdate();
    void flashButton();
    void setBadgeNumber(int number);
};

} // namespace Core

 Q_DECLARE_OPERATORS_FOR_FLAGS(Core::IOutputPane::Flags)

#endif // IOUTPUTPANE_H
