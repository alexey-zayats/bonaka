#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include "core_global.h"
#include "ioutputpane.h"
#include <QMetaType>

#include <QObject>

namespace Core {

namespace Internal { class MainWindow; }

class CORE_EXPORT MessageManager : public QObject
{
    Q_OBJECT

public:
    static QObject *instance();

    static void showOutputPane();

    enum PrintToOutputPaneFlag {
        NoModeSwitch   = IOutputPane::NoModeSwitch,
        ModeSwitch     = IOutputPane::ModeSwitch,
        WithFocus      = IOutputPane::WithFocus,
        EnsureSizeHint = IOutputPane::EnsureSizeHint,
        Silent         = 256,
        Flash          = 512
    };

    Q_DECLARE_FLAGS(PrintToOutputPaneFlags, PrintToOutputPaneFlag)

    static void write(const QString &text); // imply NoModeSwitch

public slots:
    static void write(const QString &text, Core::MessageManager::PrintToOutputPaneFlags flags);

private:
    MessageManager();
    ~MessageManager();
    static void init();
    friend class Core::Internal::MainWindow;
};

} // namespace Core

Q_DECLARE_METATYPE(Core::MessageManager::PrintToOutputPaneFlags)

#endif // MESSAGEMANAGER_H
