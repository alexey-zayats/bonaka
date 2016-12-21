#include "inavigationwidgetfactory.h"

#include <QKeySequence>

/*!
    \class Core::INavigationWidgetFactory
    \mainclass
    \inmodule Qt Creator
    \brief The INavigationWidgetFactory class provides new instances of navigation widgets.

    A navigation widget factory is necessary because there can be more than one navigation widget of
    the same type at a time. Each navigation widget is wrapped in a \l{Core::NavigationView} for
    delivery.
*/

/*!
    \class Core::NavigationView
    \inmodule Qt Creator
    \brief The NavigationView class is a C struct for wrapping a widget and a list of tool buttons.
    Wrapping the widget that is shown in the content area of the navigation widget and a list of
    tool buttons that is shown in the header above it.
*/

/*!
    \fn INavigationWidgetFactory::INavigationWidgetFactory()

    Constructs a navigation widget factory.
*/

/*!
    \fn QString INavigationWidgetFactory::displayName() const

    Returns the display name of the navigation widget, which is shown in the dropdown menu above the
    navigation widget.
*/

/*!
    \fn int INavigationWidgetFactory::priority() const

    Determines the position of the navigation widget in the dropdown menu.

    0 to 1000 from top to bottom
*/

/*!
    \fn Id INavigationWidgetFactory::id() const

    Returns a unique identifier for referencing the navigation widget factory.
*/

/*!
    \fn NavigationView INavigationWidgetFactory::createWidget()

    Returns a \l{Core::NavigationView} containing the widget and the buttons. The ownership is given
    to the caller.
*/

using namespace Core;

/*!
    Creates a \l{Core::NavigationViewFactory}.
*/
INavigationWidgetFactory::INavigationWidgetFactory()
    : m_priority(0)
{
}

/*!
    Sets the display name for the factory.

    \sa displayName()
*/
void INavigationWidgetFactory::setDisplayName(const QString &displayName)
{
    m_displayName = displayName;
}

/*!
    Sets the priority for the factory.

    \sa priority()
*/
void INavigationWidgetFactory::setPriority(int priority)
{
    m_priority = priority;
}

/*!
    Sets the id for the factory.

    \sa id()
*/
void INavigationWidgetFactory::setId(Id id)
{
    m_id = id;
}

/*!
    Sets the keyboard activation sequence for the factory.

    \sa activationSequence()
*/
void INavigationWidgetFactory::setActivationSequence(const QKeySequence &keys)
{
    m_activationSequence = keys;
}

/*!
    Returns the keyboard shortcut to activate an instance of a navigation widget.
*/
QKeySequence INavigationWidgetFactory::activationSequence() const
{
    return m_activationSequence;
}

/*!
    Stores the settings for the \a widget at \a position that was created by this factory
    (the \a position identifies a specific navigation widget).

    \sa INavigationWidgetFactory::restoreSettings()
*/
void INavigationWidgetFactory::saveSettings(int /* position */, QWidget * /* widget */)
{
}

/*!
    Reads and restores the settings for the \a widget at \a position that was created by this
    factory (the \a position identifies a specific navigation widget).

    \sa INavigationWidgetFactory::saveSettings()
*/
void INavigationWidgetFactory::restoreSettings(int /* position */, QWidget * /* widget */)
{
}
