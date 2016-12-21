
#include <QDir>
#include <QFileInfo>
#include <QLibraryInfo>
#include <QLoggingCategory>
#include <QSettings>
#include <QStyle>
#include <QTextStream>
#include <QThreadPool>
#include <QTimer>
#include <QTranslator>
#include <QUrl>
#include <QVariant>
#include <QApplication>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QNetworkProxy>
#include <QtMsgHandler>

#include "athletic.h"
#include <athletic/athletic_version.h>
#include <extensionsystem/iplugin.h>
#include <extensionsystem/pluginmanager.h>
#include <extensionsystem/pluginspec.h>

#include <utils/hostosinfo.h>
#include <utils/fileutils.h>

using namespace ExtensionSystem;

const char appName[] = "Athletic";
const char corePluginName[] = "Core";

Q_LOGGING_CATEGORY(appLog, "athletic.app")

static void displayError(const QString &t)
{
    if (Utils::HostOsInfo::isWindowsHost())
        QMessageBox::critical(0, QLatin1String(appName), t);
    else
        qCritical("%s", qPrintable(t));
}

static inline QString msgCoreLoadFailure(const QString &why)
{
    return QCoreApplication::translate("Application", "Failed to load core: %1").arg(why);
}

static inline QStringList getPluginPaths()
{
    QStringList rc;

    // Figure out root:  Up one from 'bin'
    QDir rootDir = QApplication::applicationDirPath();
    rootDir.cdUp();
    const QString rootDirPath = rootDir.canonicalPath();

    QString pluginPath;
    pluginPath = rootDirPath + QLatin1String("/plugins");
    rc.push_back(pluginPath);

    pluginPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);

    pluginPath += QLatin1String("/Athletic");
    pluginPath += QLatin1String("/plugins");

    rc.push_back(pluginPath);

    return rc;
}

static const char *SHARE_PATH =
        Utils::HostOsInfo::isMacHost() ? "/../Resources" : "/../share/athletic";

static void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString format;
    switch (type) {
        case QtDebugMsg:
            format = QLatin1Literal("%1 %2 [debug]: %3 (%4:%5, %6)\n");
            break;
        case QtInfoMsg:
            format = QLatin1Literal("%1 %2 [info]: %3 (%4:%5, %6)\n");
            break;
        case QtWarningMsg:
            format = QLatin1Literal("%1 %2 [warning]: %3 (%4:%5, %6)\n");
            break;
        case QtCriticalMsg:
            format = QLatin1Literal("%1 %2 [critical]: %3 (%4:%5, %6)\n");
            break;
        case QtFatalMsg:
            format = QLatin1Literal("%1 %2 [fatal]: %3 (%4:%5, %6)\n");
            abort();
    }

    QString date = QDateTime::currentDateTime().toString (QLatin1Literal("yyyy-MM-dd HH:mm:ss.zzz"));
    QString output = format
            .arg( date )
            .arg( QLatin1String(context.category) )
            .arg(msg)
            .arg( QLatin1String(context.file) )
            .arg( context.line )
            .arg( QLatin1String(context.function) );

    QTextStream cout(stdout);
    cout << output;
}


int main(int argc, char **argv)
{
    QLoggingCategory::setFilterRules( QLatin1String("athletic.*.debug=true\n"
                                                    "athletic.*.info=false\n"
                                                    "athletic.*.warning=true\n"
                                                    "athletic.*.critical=true") );

    qInstallMessageHandler(messageOutput);

    QApplication app(argc, argv);
    app.setApplicationName( QLatin1String(appName) );

   if (Utils::HostOsInfo().isWindowsHost()
            && !qFuzzyCompare(qApp->devicePixelRatio(), 1.0)
            && QApplication::style()->objectName().startsWith(
                QLatin1String("windows"), Qt::CaseInsensitive)) {
        QApplication::setStyle( QLatin1String("fusion") );
    }

    const int threadCount = QThreadPool::globalInstance()->maxThreadCount();
    QThreadPool::globalInstance()->setMaxThreadCount(qMax(4, 2 * threadCount));

    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    QString settingsPath;
    QStringList customPluginPaths;

    QScopedPointer<QTemporaryDir> temporaryCleanSettingsDir;

    if (!settingsPath.isEmpty())
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, settingsPath);

    // Must be done before any QSettings class is created
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope,
                       QCoreApplication::applicationDirPath() + QLatin1String(SHARE_PATH) );

    QSettings::setDefaultFormat(QSettings::IniFormat);

    // plugin manager takes control of this settings object
    QSettings *settings = new QSettings(QSettings::IniFormat,
                                        QSettings::UserScope,
                                        QLatin1String(Core::Constants::APP_SETTINGS_STR),
                                        QLatin1String("Athletic"));

    QSettings *globalSettings = new QSettings(QSettings::IniFormat,
                                              QSettings::SystemScope,
                                              QLatin1String(Core::Constants::APP_SETTINGS_STR),
                                              QLatin1String("Athletic"));

    PluginManager pluginManager;
    PluginManager::setPluginIID( QLatin1String("cloud.athletic.app.plugin") );
    PluginManager::setGlobalSettings(globalSettings);
    PluginManager::setSettings(settings);

    QTranslator translator;
    QTranslator qtTranslator;
    QStringList uiLanguages = QLocale::system().uiLanguages();
    QString overrideLanguage = settings->value( QLatin1String("General/OverrideLanguage") ).toString();

    if (!overrideLanguage.isEmpty())
        uiLanguages.prepend(overrideLanguage);

    const QString &creatorTrPath = QCoreApplication::applicationDirPath()
            + QLatin1String(SHARE_PATH) + QLatin1String("/translations");

    foreach (QString locale, uiLanguages) {
        locale = QLocale(locale).name();
        if (translator.load( QLatin1String("athletic_") + locale, creatorTrPath)) {
            const QString &qtTrPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
            const QString &qtTrFile = QLatin1String("qt_") + locale;
            // Binary installer puts Qt tr files into creatorTrPath
            if (qtTranslator.load(qtTrFile, qtTrPath) || qtTranslator.load(qtTrFile, creatorTrPath)) {
                app.installTranslator(&translator);
                app.installTranslator(&qtTranslator);
                app.setProperty("athletic_locale", locale);
                break;
            }
            translator.load(QString()); // unload()
        } else if (locale.compare( QLatin1String("C") ) == 0 ) {
            break;
        } else if (locale.startsWith( QLatin1String("en") )) {
            break;
        }
    }

    QNetworkProxyFactory::setUseSystemConfiguration(true);

    // Load
    const QStringList pluginPaths = getPluginPaths() + customPluginPaths;
    PluginManager::setPluginPaths(pluginPaths);

    const QList<PluginSpec *> plugins = PluginManager::plugins();
    PluginSpec *coreplugin = 0;
    foreach (PluginSpec *spec, plugins) {
        if (spec->name().compare( QLatin1String(corePluginName) ) == 0 ) {
            coreplugin = spec;
            break;
        }
    }
    if (!coreplugin) {
        QString nativePaths = QDir::toNativeSeparators(pluginPaths.join(QLatin1Char(',')));
        const QString reason = QCoreApplication::translate("Application", "Could not find Core plugin in %1").arg(nativePaths);
        displayError(msgCoreLoadFailure(reason));
        return 1;
    }

    if (!coreplugin->isEffectivelyEnabled()) {
        const QString reason = QCoreApplication::translate("Application", "Core plugin is disabled.");
        displayError(msgCoreLoadFailure(reason));
        return 1;
    }

    if (coreplugin->hasError()) {
        displayError(msgCoreLoadFailure(coreplugin->errorString()));
        return 1;
    }

    PluginManager::loadPlugins();
    if ( PluginManager::hasError() ) {
        displayError(msgCoreLoadFailure( QLatin1String("PluginManager::hasError() == ture") ));
        return 1;
    }
    if (coreplugin->hasError()) {
        displayError(msgCoreLoadFailure(coreplugin->errorString()));
        return 1;
    }

    // shutdown plugin manager on the exit
    QObject::connect(&app, SIGNAL(aboutToQuit()), &pluginManager, SLOT(shutdown()));

    return app.exec();
}

