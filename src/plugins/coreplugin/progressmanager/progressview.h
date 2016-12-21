#ifndef PROGRESSVIEW_H
#define PROGRESSVIEW_H

#include "progressmanager.h"

#include <QWidget>


QT_BEGIN_NAMESPACE
class QVBoxLayout;
QT_END_NAMESPACE

namespace Core {

namespace Internal {

class ProgressView : public QWidget
{
    Q_OBJECT

public:
    ProgressView(QWidget *parent = 0);
    ~ProgressView();

    void addProgressWidget(QWidget *widget);
    void removeProgressWidget(QWidget *widget);

    bool isHovered() const;

    void setReferenceWidget(QWidget *widget);

protected:
    bool event(QEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void hoveredChanged(bool hovered);

private:
    void reposition();

    QVBoxLayout *m_layout;
    QWidget *m_referenceWidget;
    bool m_hovered;
};

} // namespace Internal
} // namespace Core

#endif // PROGRESSVIEW_H
