#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSettings;
class QToolBar;
class QAction;
class QToolButton;
QT_END_NAMESPACE

namespace Core {
class SideBar;
class SideBarItem;
class Command;

namespace Internal {
class SideBarComboBox;

class SideBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SideBarWidget(SideBar *sideBar, const QString &title);
    virtual ~SideBarWidget();

    QString currentItemId() const;
    QString currentItemTitle() const;
    void setCurrentItem(const QString &id);

    void updateAvailableItems();
    void removeCurrentItem();

    Command *command(const QString &title) const;

    void setCloseIcon(const QIcon &icon);

signals:
    void splitMe();
    void closeMe();
    void currentWidgetChanged();

private:
    void setCurrentIndex(int);

    SideBarComboBox *m_comboBox;
    SideBarItem *m_currentItem;
    QToolBar *m_toolbar;
    QAction *m_splitAction;
    QAction *m_closeAction;
    QList<QAction *> m_addedToolBarActions;
    SideBar *m_sideBar;
};

} // namespace Internal
} // namespace Core

#endif // SIDEBARWIDGET_H
