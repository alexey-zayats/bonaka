#ifndef UTILS_ANSIESCAPECODEHANDLER_H
#define UTILS_ANSIESCAPECODEHANDLER_H

#include "utils_global.h"

#include <QTextCharFormat>

namespace Utils {

class UTILS_EXPORT FormattedText {
public:
    FormattedText() { }
    FormattedText(const FormattedText &other) : text(other.text), format(other.format) { }
    FormattedText(const QString &txt, const QTextCharFormat &fmt = QTextCharFormat()) :
        text(txt), format(fmt)
    { }

    QString text;
    QTextCharFormat format;
};

class UTILS_EXPORT AnsiEscapeCodeHandler
{
public:
    AnsiEscapeCodeHandler();
    QList<FormattedText> parseText(const FormattedText &input);
    void endFormatScope();

private:
    void setFormatScope(const QTextCharFormat &charFormat);

    bool            m_previousFormatClosed;
    QTextCharFormat m_previousFormat;
    QString         m_pendingText;
};

} // namespace Utils

#endif // UTILS_ANSIESCAPECODEHANDLER_H
