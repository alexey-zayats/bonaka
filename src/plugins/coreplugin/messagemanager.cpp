#include "messagemanager.h"
#include "messageoutputwindow.h"

#include <extensionsystem/pluginmanager.h>

using namespace Core;

static MessageManager *m_instance = 0;
Internal::MessageOutputWindow *m_messageOutputWindow = 0;

QObject *MessageManager::instance()
{
    return m_instance;
}

MessageManager::MessageManager()
{
    m_instance = this;
    m_messageOutputWindow = 0;
    qRegisterMetaType<MessageManager::PrintToOutputPaneFlags>();
}

MessageManager::~MessageManager()
{
    if (m_messageOutputWindow) {
        ExtensionSystem::PluginManager::removeObject(m_messageOutputWindow);
        delete m_messageOutputWindow;
    }
    m_instance = 0;
}

void MessageManager::init()
{
    m_messageOutputWindow = new Internal::MessageOutputWindow;
    ExtensionSystem::PluginManager::addObject(m_messageOutputWindow);
}

void MessageManager::showOutputPane()
{
    if (m_messageOutputWindow)
        m_messageOutputWindow->popup(IOutputPane::ModeSwitch);
}

void MessageManager::write(const QString &text)
{
    write(text, NoModeSwitch);
}

void MessageManager::write(const QString &text, PrintToOutputPaneFlags flags)
{
    if (!m_messageOutputWindow)
        return;
    if (flags & Flash) {
        m_messageOutputWindow->flash();
    } else if (flags & Silent) {
        // Do nothing
    } else {
        m_messageOutputWindow->popup(IOutputPane::Flag(int(flags)));
    }

    m_messageOutputWindow->append(text + QLatin1Char('\n'));
}

