
#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include "utils_global.h"

QT_BEGIN_NAMESPACE
class QStringList;
QT_END_NAMESPACE

namespace Utils {

// Create a usable settings key from a category,
// for example Editor|C++ -> Editor_C__
UTILS_EXPORT QString settingsKey(const QString &category);

// Return the common prefix part of a string list:
// "C:\foo\bar1" "C:\foo\bar2"  -> "C:\foo\bar"
UTILS_EXPORT QString commonPrefix(const QStringList &strings);

// Return the common path of a list of files:
// "C:\foo\bar1" "C:\foo\bar2"  -> "C:\foo"
UTILS_EXPORT QString commonPath(const QStringList &files);

// On Linux/Mac replace user's home path with ~
// Uses cleaned path and tries to use absolute path of "path" if possible
// If path is not sub of home path, or when running on Windows, returns the input
UTILS_EXPORT QString withTildeHomePath(const QString &path);

class UTILS_EXPORT AbstractMacroExpander
{
public:
    virtual ~AbstractMacroExpander() {}
    // Not const, as it may change the state of the expander.
    //! Find an expando to replace and provide a replacement string.
    //! \param str The string to scan
    //! \param pos Position to start scan on input, found position on output
    //! \param ret Replacement string on output
    //! \return Length of string part to replace, zero if no (further) matches found
    virtual int findMacro(const QString &str, int *pos, QString *ret);
    //! Provide a replacement string for an expando
    //! \param name The name of the expando
    //! \param ret Replacement string on output
    //! \return True if the expando was found
    virtual bool resolveMacro(const QString &name, QString *ret) = 0;
private:
    bool expandNestedMacros(const QString &str, int *pos, QString *ret);
};

UTILS_EXPORT void expandMacros(QString *str, AbstractMacroExpander *mx);
UTILS_EXPORT QString expandMacros(const QString &str, AbstractMacroExpander *mx);

} // namespace Utils

#endif // STRINGUTILS_H
