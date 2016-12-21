
#ifndef OUTPUTWINDOW_H
#define OUTPUTWINDOW_H

#include "core_global.h"
#include "icontext.h"

#include <utils/outputformat.h>

#include <QPlainTextEdit>
#include <QTimer>
#include <QTime>

namespace Utils { class OutputFormatter; }

namespace Core {

namespace Internal { class OutputWindowPrivate; }

class CORE_EXPORT OutputWindow : public QPlainTextEdit
{
    Q_OBJECT

public:
    OutputWindow(Context context, QWidget *parent = 0);
    ~OutputWindow();

    Utils::OutputFormatter* formatter() const;
    void setFormatter(Utils::OutputFormatter *formatter);

    void appendMessage(const QString &out, Utils::OutputFormat format);
    /// appends a \p text using \p format without using formater
    void appendText(const QString &text, const QTextCharFormat &format = QTextCharFormat());

    void grayOutOldContent();
    void clear();

    void showEvent(QShowEvent *);

    void scrollToBottom();

    void setMaxLineCount(int count);
    int maxLineCount() const;

    void setBaseFont(const QFont &newFont);
    float fontZoom() const;
    void setFontZoom(float zoom);
    void setWheelZoomEnabled(bool enabled);

signals:
    void wheelZoom();

public slots:
    void setWordWrapEnabled(bool wrap);

protected:
    bool isScrollbarAtBottom() const;

    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void resizeEvent(QResizeEvent *e);
    virtual void keyPressEvent(QKeyEvent *ev);
    virtual void wheelEvent(QWheelEvent *e);

private:
    using QPlainTextEdit::setFont; // call setBaseFont instead, which respects the zoom factor
    QTimer m_scrollTimer;
    QTime m_lastMessage;
    void enableUndoRedo();
    QString doNewlineEnforcement(const QString &out);

    Internal::OutputWindowPrivate *d;
};

} // namespace Core

#endif // OUTPUTWINDOW_H
