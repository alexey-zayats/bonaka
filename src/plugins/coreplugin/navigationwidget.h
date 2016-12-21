#ifndef NAVIGATIONWIDGET_H
#define NAVIGATIONWIDGET_H

#include <coreplugin/minisplitter.h>
#include <coreplugin/id.h>

#include <QHash>

QT_BEGIN_NAMESPACE
class QSettings;
class QAbstractItemModel;
class QStandardItemModel;
QT_END_NAMESPACE

namespace Core {
class INavigationWidgetFactory;
class IMode;
class Command;
class NavigationWidget;
struct NavigationWidgetPrivate;
namespace Internal { class NavigationSubWidget; }

class CORE_EXPORT NavigationWidgetPlaceHolder : public QWidget
{
    Q_OBJECT
    friend class Core::NavigationWidget;

public:
    explicit NavigationWidgetPlaceHolder(IMode *mode, QWidget *parent = 0);
    virtual ~NavigationWidgetPlaceHolder();
    static NavigationWidgetPlaceHolder* current();
    void applyStoredSize(int width);

private:
    void currentModeAboutToChange(IMode *);

    IMode *m_mode;
    static NavigationWidgetPlaceHolder* m_current;
};

class CORE_EXPORT NavigationWidget : public MiniSplitter
{
    Q_OBJECT

public:
    enum FactoryModelRoles {
        FactoryObjectRole = Qt::UserRole,
        FactoryIdRole,
        FactoryPriorityRole
    };

    explicit NavigationWidget(QAction *toggleSideBarAction);
    virtual ~NavigationWidget();

    void setFactories(const QList<INavigationWidgetFactory*> &factories);

    void saveSettings(QSettings *settings);
    void restoreSettings(QSettings *settings);

    QWidget *activateSubWidget(Id factoryId);
    void closeSubWidgets();

    bool isShown() const;
    void setShown(bool b);

    bool isSuppressed() const;
    void setSuppressed(bool b);

    static NavigationWidget* instance();

    int storedWidth();

    // Called from the place holders
    void placeHolderChanged(NavigationWidgetPlaceHolder *holder);

    QHash<Id, Command *> commandMap() const;
    QAbstractItemModel *factoryModel() const;

protected:
    void resizeEvent(QResizeEvent *);

private:
    void splitSubWidget(int factoryIndex);
    void closeSubWidget();
    void updateToggleText();
    Internal::NavigationSubWidget *insertSubItem(int position, int index);
    int factoryIndex(Id id);

    NavigationWidgetPrivate *d;
};

} // namespace Core

#endif // NAVIGATIONWIDGET_H
