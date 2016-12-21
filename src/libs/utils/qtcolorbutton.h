
#ifndef QTCOLORBUTTON_H
#define QTCOLORBUTTON_H

#include "utils_global.h"

#include <QToolButton>

namespace Utils {

class UTILS_EXPORT QtColorButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(bool backgroundCheckered READ isBackgroundCheckered WRITE setBackgroundCheckered)
    Q_PROPERTY(bool alphaAllowed READ isAlphaAllowed WRITE setAlphaAllowed)
    Q_PROPERTY(QColor color READ color WRITE setColor)
public:
    QtColorButton(QWidget *parent = 0);
    ~QtColorButton();

    bool isBackgroundCheckered() const;
    void setBackgroundCheckered(bool checkered);

    bool isAlphaAllowed() const;
    void setAlphaAllowed(bool allowed);

    QColor color() const;

public slots:
    void setColor(const QColor &color);

signals:
    void colorChanged(const QColor &color);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
#ifndef QT_NO_DRAGANDDROP
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
#endif
private:
    class QtColorButtonPrivate *d_ptr;
    friend class QtColorButtonPrivate;
};

} // namespace Utils

#endif // QTCOLORBUTTON_H
