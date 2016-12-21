#ifndef TIPS_H
#define TIPS_H

#include "../utils_global.h"

#include <QLabel>
#include <QPixmap>
#include <QSharedPointer>
#include <QVariant>
#include <QVBoxLayout>

#ifndef Q_MOC_RUN
namespace Utils {
namespace Internal {
#endif

// Please do not change the name of this class. Detailed comments in tooltip.h.
class QTipLabel : public QLabel
{
    Q_OBJECT
public:
    QTipLabel(QWidget *parent);

    virtual void setContent(const QVariant &content) = 0;
    virtual bool isInteractive() const { return false; }
    virtual int showTime() const = 0;
    virtual void configure(const QPoint &pos, QWidget *w) = 0;
    virtual bool canHandleContentReplacement(int typeId) const = 0;
    virtual bool equals(int typeId, const QVariant &other, const QString &helpId) const = 0;
    virtual void setHelpId(const QString &id);
    virtual QString helpId() const;

private:
    QString m_helpId;
};

class TextTip : public QTipLabel
{
public:
    TextTip(QWidget *parent);

    virtual void setContent(const QVariant &content);
    virtual void configure(const QPoint &pos, QWidget *w);
    virtual bool canHandleContentReplacement(int typeId) const;
    virtual int showTime() const;
    virtual bool equals(int typeId, const QVariant &other, const QString &otherHelpId) const;
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

private:
    QString m_text;
};

class ColorTip : public QTipLabel
{
public:
    ColorTip(QWidget *parent);

    virtual void setContent(const QVariant &content);
    virtual void configure(const QPoint &pos, QWidget *w);
    virtual bool canHandleContentReplacement(int typeId) const;
    virtual int showTime() const { return 4000; }
    virtual bool equals(int typeId, const QVariant &other, const QString &otherHelpId) const;
    virtual void paintEvent(QPaintEvent *event);

private:
    QColor m_color;
    QPixmap m_tilePixmap;
};

class WidgetTip : public QTipLabel
{
    Q_OBJECT

public:
    explicit WidgetTip(QWidget *parent = 0);
    void pinToolTipWidget(QWidget *parent);

    virtual void setContent(const QVariant &content);
    virtual void configure(const QPoint &pos, QWidget *w);
    virtual bool canHandleContentReplacement(int typeId) const;
    virtual int showTime() const { return 30000; }
    virtual bool equals(int typeId, const QVariant &other, const QString &otherHelpId) const;
    virtual bool isInteractive() const { return true; }

private:
    QWidget *m_widget;
    QVBoxLayout *m_layout;
};

#ifndef Q_MOC_RUN
} // namespace Internal
} // namespace Utils
#endif

#endif // TIPS_H
