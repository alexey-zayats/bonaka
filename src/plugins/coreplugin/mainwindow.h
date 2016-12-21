#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <utils/appmainwindow.h>

#include "icontext.h"
#include "icore.h"

#include <QMap>
#include <QColor>

#include <functional>

QT_BEGIN_NAMESPACE
class QSettings;
class QPrinter;
class QToolButton;
QT_END_NAMESPACE

namespace Core {

class StatusBarWidget;
class ModeManager;
class NavigationWidget;
class RightPaneWidget;
class SettingsDatabase;
class EditorManager;
class MessageManager;

namespace Internal {

class TabWidget;
class GeneralSettings;
class ShortcutSettings;
class StatusBarManager;
class VersionDialog;
class WindowSupport;
class SystemSettings;
class ProgressManagerPrivate;

class MainWindow : public Utils::AppMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    bool init(QString *errorMessage);
    void extensionsInitialized();
    void aboutToShutdown();

    IContext *contextObject(QWidget *widget);
    void addContextObject(IContext *context);
    void removeContextObject(IContext *context);

    void updateAdditionalContexts(const Context &remove, const Context &add,
                                      ICore::ContextPriority priority);
    inline SettingsDatabase *settingsDatabase() const { return m_settingsDatabase; }
    virtual QPrinter *printer() const;

    IContext * currentContextObject() const;

    QStatusBar *statusBar() const;

    void setOverrideColor(const QColor &color);

    QStringList additionalAboutInformation() const;
    void appendAboutInformation(const QString &line);

    void addPreCloseListener(const std::function<bool()> &listener);
    void setSuppressNavigationWidget(bool suppress);

    TabWidget *tabWidget() const { return m_modeStack; }


public slots:
    void exit();
    bool showOptionsDialog(Id page = Id(), QWidget *parent = 0);
    bool showWarningWithOptions(const QString &title, const QString &text,
                                    const QString &details = QString(),
                                    Id settingsId = Id(),
                                    QWidget *parent = 0);
    void aboutApplication();

protected:
    virtual void closeEvent(QCloseEvent *event);

private:
    void updateFocusWidget(QWidget *old, QWidget *now);
    void aboutPlugins();
    void setSidebarVisible(bool visible);
    void destroyVersionDialog();
    void restoreWindowState();

    void setFocusToEditor();

    void updateContextObject(const QList<IContext *> &context);
    void updateContext();

    void readSettings();
    void writeSettings();

    void registerDefaultContainers();
    void registerDefaultActions();

    ICore *m_coreImpl;

    Context m_highPrioAdditionalContexts;
    Context m_lowPrioAdditionalContexts;

    QStringList m_aboutInformation;

    SettingsDatabase *m_settingsDatabase;

    StatusBarWidget *m_outputView;
    WindowSupport *m_windowSupport;
    EditorManager *m_editorManager;
    MessageManager *m_messageManager;
    ProgressManagerPrivate *m_progressManager;
    StatusBarManager *m_statusBarManager;
    ModeManager *m_modeManager;
    TabWidget *m_modeStack;
    NavigationWidget *m_navigationWidget;

    RightPaneWidget *m_rightPaneWidget;
    VersionDialog *m_versionDialog;
    mutable QPrinter *m_printer;

    GeneralSettings *m_generalSettings;
    SystemSettings *m_systemSettings;
    ShortcutSettings *m_shortcutSettings;

    QList<IContext *> m_activeContext;

    QMap<QWidget *, IContext *> m_contextWidgets;

    QAction *m_focusToEditor;
    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_exitAction;
    QAction *m_optionsAction;
    QAction *m_toggleSideBarAction;
    QToolButton *m_toggleSideBarButton;

    QAction *m_toggleModeSelectorAction;

    QColor m_overrideColor;
    QList<std::function<bool()>> m_preCloseListeners;

    QAction *m_projectSelectorAction;
};

} // namespace Internal
} // namespace Core

#endif // MAINWINDOW_H
