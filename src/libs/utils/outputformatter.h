
#ifndef OUTPUTFORMATTER_H
#define OUTPUTFORMATTER_H

#include "utils_global.h"
#include "outputformat.h"

#include <QObject>
#include <QFont>

QT_BEGIN_NAMESPACE
class QPlainTextEdit;
class QTextCharFormat;
class QTextCursor;
QT_END_NAMESPACE

namespace Utils {

class FormattedText;

namespace Internal { class OutputFormatterPrivate; }

class UTILS_EXPORT OutputFormatter : public QObject
{
    Q_OBJECT

public:
    OutputFormatter();
    virtual ~OutputFormatter();

    QPlainTextEdit *plainTextEdit() const;
    virtual void setPlainTextEdit(QPlainTextEdit *plainText);

    void flush();

    virtual void appendMessage(const QString &text, OutputFormat format);
    virtual void appendMessage(const QString &text, const QTextCharFormat &format);
    virtual void handleLink(const QString &href);

protected:
    void initFormats();
    virtual void clearLastLine();
    QTextCharFormat charFormat(OutputFormat format) const;
    QList<Utils::FormattedText> parseAnsi(const QString &text, const QTextCharFormat &format);
    void append(QTextCursor &cursor, const QString &text, const QTextCharFormat &format);

private:
    Internal::OutputFormatterPrivate *d;
};

} // namespace Utils

#endif // OUTPUTFORMATTER_H
