
#ifndef MESSAGEOUTPUTWINDOW_H
#define MESSAGEOUTPUTWINDOW_H

#include "ioutputpane.h"

namespace Core {
class OutputWindow;

namespace Internal {

class MessageOutputWindow : public IOutputPane
{
    Q_OBJECT

public:
    MessageOutputWindow();
    ~MessageOutputWindow();

    QWidget *outputWidget(QWidget *parent);
    QList<QWidget*> toolBarWidgets() const { return QList<QWidget *>(); }

    QString displayName() const;
    int priorityInStatusBar() const;
    void clearContents();
    void visibilityChanged(bool visible);

    void append(const QString &text);
    bool canFocus() const;
    bool hasFocus() const;
    void setFocus();

    virtual bool canNext() const;
    virtual bool canPrevious() const;
    virtual void goToNext();
    virtual void goToPrev();
    bool canNavigate() const;

private:
    OutputWindow *m_widget;
};

} // namespace Internal
} // namespace Core

#endif // MESSAGEOUTPUTWINDOW_H
