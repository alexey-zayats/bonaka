
#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QIcon>
#include <QWidget>

#include <QTimer>
#include <QPropertyAnimation>

QT_BEGIN_NAMESPACE
class QPainter;
class QStackedLayout;
class QStatusBar;
QT_END_NAMESPACE

namespace Core {
namespace Internal {

class Tab : public QObject
{
    Q_OBJECT

    Q_PROPERTY(float fader READ fader WRITE setFader)
public:
    Tab(QWidget *tabbar) : enabled(false), tabbar(tabbar), m_fader(0) {
        animator.setPropertyName("fader");
        animator.setTargetObject(this);
    }
    float fader() { return m_fader; }
    void setFader(float value);

    void fadeIn();
    void fadeOut();

    QIcon icon;
    QString text;
    QString toolTip;
    bool enabled;

private:
    QPropertyAnimation animator;
    QWidget *tabbar;
    float m_fader;
};

class TabBar : public QWidget
{
    Q_OBJECT

public:
    TabBar(QWidget *parent = 0);
    ~TabBar();

    bool event(QEvent *event);

    void paintEvent(QPaintEvent *event);
    void paintTab(QPainter *painter, int tabIndex) const;
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    bool validIndex(int index) const { return index >= 0 && index < m_tabs.count(); }

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    void setTabEnabled(int index, bool enable);
    bool isTabEnabled(int index) const;

    void insertTab(int index, const QIcon &icon, const QString &label) {
        Tab *tab = new Tab(this);
        tab->icon = icon;
        tab->text = label;
        m_tabs.insert(index, tab);
        updateGeometry();
    }

    void removeTab(int index) {
        Tab *tab = m_tabs.takeAt(index);
        delete tab;
        updateGeometry();
    }
    void setCurrentIndex(int index);
    int currentIndex() const { return m_currentIndex; }

    void setTabToolTip(int index, QString toolTip) { m_tabs[index]->toolTip = toolTip; }
    QString tabToolTip(int index) const { return m_tabs.at(index)->toolTip; }

    QIcon tabIcon(int index) const { return m_tabs.at(index)->icon; }
    QString tabText(int index) const { return m_tabs.at(index)->text; }
    int count() const {return m_tabs.count(); }
    QRect tabRect(int index) const;

signals:
    void currentChanged(int);

public slots:
    void emitCurrentIndex();

private:
    static const int m_rounding;
    static const int m_textPadding;
    QRect m_hoverRect;
    int m_hoverIndex;
    int m_currentIndex;
    QList<Tab*> m_tabs;
    QTimer m_triggerTimer;
    QSize tabSizeHint(bool minimum = false) const;

};

class TabWidget : public QWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget *parent = 0);

    void insertTab(int index, QWidget *tab, const QIcon &icon, const QString &label);
    void removeTab(int index);
    void setBackgroundBrush(const QBrush &brush);
    void addCornerWidget(QWidget *widget);
    void insertCornerWidget(int pos, QWidget *widget);
    int cornerWidgetCount() const;
    void setTabToolTip(int index, const QString &toolTip);

    void paintEvent(QPaintEvent *event);

    int currentIndex() const;
    QStatusBar *statusBar() const;

    void setTabEnabled(int index, bool enable);
    bool isTabEnabled(int index) const;

    bool isSelectionWidgetVisible() const;

signals:
    void currentAboutToShow(int index);
    void currentChanged(int index);
    void topAreaClicked(Qt::MouseButton button, Qt::KeyboardModifiers modifiers);

public slots:
    void setCurrentIndex(int index);
    void setSelectionWidgetVisible(bool visible);

private:
    void showWidget(int index);

    TabBar *m_tabBar;
    QWidget *m_cornerWidgetContainer;
    QStackedLayout *m_modesStack;
    QWidget *m_selectionWidget;
    QStatusBar *m_statusBar;
};

class ColorButton : public QWidget
{
    Q_OBJECT

public:
    explicit ColorButton(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *event);

signals:
    void clicked(Qt::MouseButton button, Qt::KeyboardModifiers modifiers);
};


} // namespace Internal
} // namespace Core

#endif // TABWIDGET_H
