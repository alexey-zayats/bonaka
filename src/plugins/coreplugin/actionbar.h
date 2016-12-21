#ifndef ACTIONBAR_H
#define ACTIONBAR_H

#include <QToolButton>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
QT_END_NAMESPACE

namespace Core {
namespace Internal {

class ToolButton : public QToolButton
{
    Q_OBJECT

    Q_PROPERTY(float fader READ fader WRITE setFader)

public:
    ToolButton(QAction *action, QWidget *parent = 0);

    void paintEvent(QPaintEvent *event);
    bool event(QEvent *e);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    float m_fader;
    float fader() { return m_fader; }
    void setFader(float value) { m_fader = value; update(); }

    static void hoverOverlay(QPainter *painter, const QRect &spanRect);

private:
    void actionChanged();
};

class ActionBar : public QWidget
{
    Q_OBJECT

public:
    ActionBar(QWidget *parent = 0);

    void paintEvent(QPaintEvent *event);
    void insertAction(int index, QAction *action);
    void addProjectSelector(QAction *action);
    QLayout *actionsLayout() const;
    QSize minimumSizeHint() const;

private:
    QVBoxLayout *m_actionsLayout;
};

} // namespace Internal
} // namespace Core

#endif // ACTIONBAR_H
