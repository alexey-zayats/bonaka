
#ifndef OUTPUTPANE_H
#define OUTPUTPANE_H

#include "core_global.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSplitter;
QT_END_NAMESPACE

namespace Core {

class IMode;

namespace Internal { class OutputPaneManager; }
class OutputPanePlaceHolderPrivate;

class CORE_EXPORT OutputPanePlaceHolder : public QWidget
{
    Q_OBJECT

public:
    explicit OutputPanePlaceHolder(IMode *mode, QSplitter *parent = 0);
    ~OutputPanePlaceHolder();

    static OutputPanePlaceHolder *getCurrent();
    static bool isCurrentVisible();

    bool isMaximized() const;
    void setMaximized(bool maximize);
    void ensureSizeHintAsMinimum();
    int nonMaximizedSize() const;

protected:
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *);

private:
    void setHeight(int height);
    void currentModeChanged(IMode *);

    OutputPanePlaceHolderPrivate *d;
};

} // namespace Core

#endif // OUTPUTPANE_H
