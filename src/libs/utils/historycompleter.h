#ifndef HISTORYCOMPLETER_H
#define HISTORYCOMPLETER_H

#include "utils_global.h"

#include <QCompleter>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace Utils {

namespace Internal { class HistoryCompleterPrivate; }

class UTILS_EXPORT HistoryCompleter : public QCompleter
{
    Q_OBJECT

public:
    static void setSettings(QSettings *settings);
    HistoryCompleter(const QString &historyKey, QObject *parent = 0);
    bool removeHistoryItem(int index);
    QString historyItem() const;
    static bool historyExistsFor(const QString &historyKey);

private:
    ~HistoryCompleter();
    int historySize() const;
    int maximalHistorySize() const;
    void setMaximalHistorySize(int numberOfEntries);

public Q_SLOTS:
    void clearHistory();
    void addEntry(const QString &str);

private:
    Internal::HistoryCompleterPrivate *d;
};

} // namespace Utils

#endif // HISTORYCOMPLETER_H
