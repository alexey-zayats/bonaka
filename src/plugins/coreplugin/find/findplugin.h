#ifndef FINDPLUGIN_H
#define FINDPLUGIN_H

#include "textfindconstants.h"

#include <QObject>

QT_BEGIN_NAMESPACE
class QStringListModel;
QT_END_NAMESPACE

namespace Core {
class IFindFilter;
class FindPluginPrivate;

namespace Internal {
class CorePlugin;
class FindToolBar;
class CurrentDocumentFind;
} // namespace Internal

class CORE_EXPORT FindPlugin : public QObject
{
    Q_OBJECT

public:
    FindPlugin();
    virtual ~FindPlugin();

    static FindPlugin *instance();

    enum FindDirection {
        FindForwardDirection,
        FindBackwardDirection
    };

    FindFlags findFlags() const;
    bool hasFindFlag(FindFlag flag);
    void updateFindCompletion(const QString &text);
    void updateReplaceCompletion(const QString &text);
    QStringListModel *findCompletionModel() const;
    QStringListModel *replaceCompletionModel() const;
    void setUseFakeVim(bool on);
    void openFindToolBar(FindDirection direction);
    void openFindDialog(IFindFilter *filter);

    void initialize(const QStringList &, QString *);
    void extensionsInitialized();
    void aboutToShutdown();

public slots:
    void setCaseSensitive(bool sensitive);
    void setWholeWord(bool wholeOnly);
    void setBackward(bool backward);
    void setRegularExpression(bool regExp);
    void setPreserveCase(bool preserveCase);

signals:
    void findFlagsChanged();

private:
    void filterChanged();
    void displayNameChanged();
    void openFindFilter();
    void writeSettings();
    void setFindFlag(Core::FindFlag flag, bool enabled);
    void updateCompletion(const QString &text, QStringList &completions, QStringListModel *model);
    void setupMenu();
    void setupFilterMenuItems();
    void readSettings();

    //variables
    FindPluginPrivate *d;
};

} // namespace Core

#endif // FINDPLUGIN_H
