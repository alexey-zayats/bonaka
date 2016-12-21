#include "ieditor.h"

/*!
  \class Core::IEditor
  \brief The IEditor class is an interface for providing different editors for
  different file types.

  Classes that implement this interface are for example the editors for
  C++ files, UI files and resource files.

  Whenever a user wants to edit or create a file, the EditorManager scans all
  EditorFactoryInterfaces for suitable editors. The selected EditorFactory
  is then asked to create an editor, which must implement this interface.

  Guidelines for implementing:
  \list
  \li \c displayName() is used as a user visible description of the document
      (usually filename w/o path).
  \li \c kind() must be the same value as the \c kind() of the corresponding
      EditorFactory.
  \li If duplication is supported, you need to ensure that all duplicates
        return the same \c file().
  \li QString \c preferredMode() const is the mode the editor manager should
      activate. Some editors use a special mode (such as \gui Design mode).
  \endlist

  \sa Core::EditorFactoryInterface Core::IContext

*/

namespace Core {

IEditor::IEditor(QObject *parent)
    : IContext(parent), m_duplicateSupported(false)
{}

bool IEditor::duplicateSupported() const
{
    return m_duplicateSupported;
}

void IEditor::setDuplicateSupported(bool duplicatesSupported)
{
    m_duplicateSupported = duplicatesSupported;
}

} // namespace Core
