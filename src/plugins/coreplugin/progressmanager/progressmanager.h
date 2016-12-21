
#ifndef PROGRESSMANAGER_H
#define PROGRESSMANAGER_H

#include <coreplugin/core_global.h>
#include <coreplugin/id.h>

#include <QFuture>
#include <QFutureInterfaceBase>
#include <QObject>

QT_FORWARD_DECLARE_CLASS(QTimer)

namespace Core {
class FutureProgress;

namespace Internal { class ProgressManagerPrivate; }

class CORE_EXPORT ProgressManager : public QObject
{
    Q_OBJECT
public:
    enum ProgressFlag {
        KeepOnFinish = 0x01,
        ShowInApplicationIcon = 0x02
    };
    Q_DECLARE_FLAGS(ProgressFlags, ProgressFlag)

    static ProgressManager *instance();

    static FutureProgress *addTask(const QFuture<void> &future, const QString &title,
                                   Id type, ProgressFlags flags = 0);
    static FutureProgress *addTimedTask(const QFutureInterface<void> &fi, const QString &title,
                                   Id type, int expectedSeconds, ProgressFlags flags = 0);
    static void setApplicationLabel(const QString &text);

public slots:
    static void cancelTasks(Id type);

signals:
    void taskStarted(Core::Id type);
    void allTasksFinished(Core::Id type);

private:
    ProgressManager();
    ~ProgressManager();

    friend class Core::Internal::ProgressManagerPrivate;
};

class CORE_EXPORT ProgressTimer : public QObject
{
public:
    ProgressTimer(const QFutureInterfaceBase &futureInterface, int expectedSeconds,
                  QObject *parent = 0);

private:
    void handleTimeout();

    QFutureInterfaceBase m_futureInterface;
    int m_expectedTime;
    int m_currentTime;
    QTimer *m_timer;
};

} // namespace Core

Q_DECLARE_OPERATORS_FOR_FLAGS(Core::ProgressManager::ProgressFlags)

#endif //PROGRESSMANAGER_H
