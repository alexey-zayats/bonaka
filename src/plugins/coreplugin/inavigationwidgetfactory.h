#ifndef INAVIGATIONWIDGET_H
#define INAVIGATIONWIDGET_H

#include "id.h"

#include <QObject>
#include <QList>
#include <QKeySequence>

QT_BEGIN_NAMESPACE
class QToolButton;
class QWidget;
QT_END_NAMESPACE

namespace Core {

struct NavigationView
{
    NavigationView(QWidget *w = 0) : widget(w) {}

    QWidget *widget;
    QList<QToolButton *> dockToolBarWidgets;
};

class CORE_EXPORT INavigationWidgetFactory : public QObject
{
    Q_OBJECT

public:
    INavigationWidgetFactory();

    void setDisplayName(const QString &displayName);
    void setPriority(int priority);
    void setId(Id id);
    void setActivationSequence(const QKeySequence &keys);

    QString displayName() const { return m_displayName ; }
    int priority() const { return m_priority; }
    Id id() const { return m_id; }
    QKeySequence activationSequence() const;

    // This design is not optimal, think about it again once we need to extend it
    // It could be implemented as returning an object which has both the widget
    // and the docktoolbar widgets
    // Similar to how IView
    virtual NavigationView createWidget() = 0;

    virtual void saveSettings(int position, QWidget *widget);
    virtual void restoreSettings(int position, QWidget *widget);

private:
    QString m_displayName;
    int m_priority;
    Id m_id;
    QKeySequence m_activationSequence;
};

} // namespace Core

#endif // INAVIGATIONWIDGET_H
