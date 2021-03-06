#ifndef INFOBAR_H
#define INFOBAR_H

#include "core_global.h"
#include <coreplugin/id.h>

#include <QObject>
#include <QSet>

#include <functional>

QT_BEGIN_NAMESPACE
class QBoxLayout;
QT_END_NAMESPACE

namespace Core {

class InfoBar;
class InfoBarDisplay;

class CORE_EXPORT InfoBarEntry
{
public:
    enum GlobalSuppressionMode
    {
        GlobalSuppressionDisabled,
        GlobalSuppressionEnabled
    };

    InfoBarEntry(Id _id, const QString &_infoText, GlobalSuppressionMode _globalSuppression = GlobalSuppressionDisabled);
    InfoBarEntry(const InfoBarEntry &other) { *this = other; }

    typedef std::function<void()> CallBack;
    void setCustomButtonInfo(const QString &_buttonText, CallBack callBack);
    void setCancelButtonInfo(CallBack callBack);
    void setCancelButtonInfo(const QString &_cancelButtonText, CallBack callBack);

private:
    Id id;
    QString infoText;
    QString buttonText;
    CallBack m_buttonCallBack;
    QString cancelButtonText;
    CallBack m_cancelButtonCallBack;
    GlobalSuppressionMode globalSuppression;
    friend class InfoBar;
    friend class InfoBarDisplay;
};

class CORE_EXPORT InfoBar : public QObject
{
    Q_OBJECT

public:
    void addInfo(const InfoBarEntry &info);
    void removeInfo(Id id);
    bool containsInfo(Id id) const;
    void suppressInfo(Id id);
    bool canInfoBeAdded(Id id) const;
    void enableInfo(Id id);
    void clear();
    static void globallySuppressInfo(Id id);
    static void initializeGloballySuppressed();
    static void clearGloballySuppressed();
    static bool anyGloballySuppressed();

signals:
    void changed();

private:
    QList<InfoBarEntry> m_infoBarEntries;
    QSet<Id> m_suppressed;
    static QSet<Id> globallySuppressed;
    friend class InfoBarDisplay;
};

class CORE_EXPORT InfoBarDisplay : public QObject
{
    Q_OBJECT

public:
    InfoBarDisplay(QObject *parent = 0);
    void setTarget(QBoxLayout *layout, int index);
    void setInfoBar(InfoBar *infoBar);

private:
    void update();
    void infoBarDestroyed();
    void widgetDestroyed();

    QList<QWidget *> m_infoWidgets;
    InfoBar *m_infoBar;
    QBoxLayout *m_boxLayout;
    int m_boxIndex;
};

} // namespace Core

#endif // INFOBAR_H
