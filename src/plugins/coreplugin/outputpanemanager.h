
#ifndef OUTPUTPANEMANAGER_H
#define OUTPUTPANEMANAGER_H

#include <coreplugin/id.h>

#include <QMap>
#include <QToolButton>

QT_BEGIN_NAMESPACE
class QAction;
class QComboBox;
class QLabel;
class QSplitter;
class QStackedWidget;
class QTimeLine;
class QLabel;
QT_END_NAMESPACE

namespace Core {

class IOutputPane;

namespace Internal {

class MainWindow;
class OutputPaneToggleButton;
class OutputPaneManageButton;

class OutputPaneManager : public QWidget
{
    Q_OBJECT

public:
    void init();
    static OutputPaneManager *instance();
    QWidget *buttonsWidget();
    void updateStatusButtons(bool visible);
    static void updateMaximizeButton(bool maximized);

    static int outputPaneHeightSetting();
    static void setOutputPaneHeightSetting(int value);

public slots:
    void slotHide();
    void slotNext();
    void slotPrev();
    void shortcutTriggered();
    void toggleMaximized();

protected:
    void focusInEvent(QFocusEvent *e);

private:
    // the only class that is allowed to create and destroy
    friend class MainWindow;
    friend class OutputPaneManageButton;

    static void create();
    static void destroy();

    explicit OutputPaneManager(QWidget *parent = 0);
    ~OutputPaneManager();

    void togglePage(int flags);
    void clearPage();
    void updateNavigateState();
    void popupMenu();
    void saveSettings() const;
    void flashButton();
    void setBadgeNumber(int number);
    void showPage(int idx, int flags);
    void ensurePageVisible(int idx);
    int findIndexForPage(IOutputPane *out);
    int currentIndex() const;
    void setCurrentIndex(int idx);
    void buttonTriggered(int idx);
    void readSettings();

    QLabel *m_titleLabel;
    OutputPaneManageButton *m_manageButton;
    QAction *m_clearAction;
    QToolButton *m_clearButton;
    QToolButton *m_closeButton;

    QAction *m_minMaxAction;
    QToolButton *m_minMaxButton;

    QAction *m_nextAction;
    QAction *m_prevAction;
    QToolButton *m_prevToolButton;
    QToolButton *m_nextToolButton;
    QWidget *m_toolBar;

    QList<IOutputPane *> m_panes;
    QVector<OutputPaneToggleButton *> m_buttons;
    QVector<QAction *> m_actions;
    QVector<Id> m_ids;
    QMap<Id, bool> m_buttonVisibility;

    QStackedWidget *m_outputWidgetPane;
    QStackedWidget *m_opToolBarWidgets;
    QWidget *m_buttonsWidget;
    QIcon m_minimizeIcon;
    QIcon m_maximizeIcon;
    bool m_maximized;
    int m_outputPaneHeightSetting;
};

class BadgeLabel
{
public:
    BadgeLabel();
    void paint(QPainter *p, int x, int y, bool isChecked);
    void setText(const QString &text);
    QString text() const;
    QSize sizeHint() const;

private:
    void calculateSize();

    QSize m_size;
    QString m_text;
    QFont m_font;
    static const int m_padding = 6;
};

class OutputPaneToggleButton : public QToolButton
{
    Q_OBJECT
public:
    OutputPaneToggleButton(int number, const QString &text, QAction *action,
                           QWidget *parent = 0);
    QSize sizeHint() const;
    void paintEvent(QPaintEvent*);
    void flash(int count = 3);
    void setIconBadgeNumber(int number);

private:
    void updateToolTip();
    void checkStateSet();

    QString m_number;
    QString m_text;
    QAction *m_action;
    QTimeLine *m_flashTimer;
    BadgeLabel m_badgeNumberLabel;
};

class OutputPaneManageButton : public QToolButton
{
    Q_OBJECT
public:
    OutputPaneManageButton();
    QSize sizeHint() const;
    void paintEvent(QPaintEvent*);
};

} // namespace Internal
} // namespace Core

#endif // OUTPUTPANEMANAGER_H
