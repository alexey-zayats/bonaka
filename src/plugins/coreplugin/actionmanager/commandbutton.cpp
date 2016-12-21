
#include "commandbutton.h"
#include "actionmanager.h"
#include "command.h"
#include "../id.h"

#include <utils/proxyaction.h>

using namespace Core;

/*!
    \class Core::CommandButton

    \brief The CommandButton class is a tool button associated with one of
    the registered Command objects.

    Tooltip of this button consists of toolTipBase property value and Command's
    key sequence which is automatically updated when user changes it.
 */

CommandButton::CommandButton(QWidget *parent)
    : QToolButton(parent)
    , m_command(0)
{
}

CommandButton::CommandButton(Id id, QWidget *parent)
    : QToolButton(parent)
    , m_command(0)
{
    setCommandId(id);
}

void CommandButton::setCommandId(Id id)
{
    if (m_command)
        disconnect(m_command.data(), &Command::keySequenceChanged, this, &CommandButton::updateToolTip);

    m_command = ActionManager::command(id);

    if (m_toolTipBase.isEmpty())
        m_toolTipBase = m_command->description();

    updateToolTip();
    connect(m_command.data(), &Command::keySequenceChanged, this, &CommandButton::updateToolTip);
}

QString CommandButton::toolTipBase() const
{
    return m_toolTipBase;
}

void CommandButton::setToolTipBase(const QString &toolTipBase)
{
    m_toolTipBase = toolTipBase;
    updateToolTip();
}

void CommandButton::updateToolTip()
{
    if (m_command)
        setToolTip(Utils::ProxyAction::stringWithAppendedShortcut(m_toolTipBase,
                                                                  m_command->keySequence()));
}
