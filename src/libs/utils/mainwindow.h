
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "utils_global.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace Utils {

struct MainWindowPrivate;

class UTILS_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    /* The widget passed in should have an objectname set
     * which will then be used as key for QSettings. */
    QDockWidget *addDockForWidget(QWidget *widget);
    QList<QDockWidget *> dockWidgets() const;

    void setTrackingEnabled(bool enabled);

    void saveSettings(QSettings *settings) const;
    void restoreSettings(const QSettings *settings);
    QHash<QString, QVariant> saveSettings() const;
    void restoreSettings(const QHash<QString, QVariant> &settings);

    // Additional context menu actions
    QAction *menuSeparator1() const;
    QAction *autoHideTitleBarsAction() const;
    QAction *menuSeparator2() const;
    QAction *resetLayoutAction() const;
    void addDockActionsToMenu(QMenu *menu);

    bool autoHideTitleBars() const;

signals:
    // Emitted by resetLayoutAction(). Connect to a slot
    // restoring the default layout.
    void resetLayout();

public slots:
    void setDockActionsVisible(bool v);

protected:
    void hideEvent(QHideEvent *event);
    void showEvent(QShowEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void onDockActionTriggered();

private:
    void handleVisibilityChanged(bool visible);

    MainWindowPrivate *d;
};

} // namespace Utils

#endif // MAINWINDOW_H
