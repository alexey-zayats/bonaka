#ifndef BASETEXTFIND_H
#define BASETEXTFIND_H

#include "ifindsupport.h"

QT_BEGIN_NAMESPACE
class QPlainTextEdit;
class QTextEdit;
class QTextCursor;
QT_END_NAMESPACE

namespace Core {
struct BaseTextFindPrivate;

class CORE_EXPORT BaseTextFind : public IFindSupport
{
    Q_OBJECT

public:
    explicit BaseTextFind(QPlainTextEdit *editor);
    explicit BaseTextFind(QTextEdit *editor);
    virtual ~BaseTextFind();

    bool supportsReplace() const;
    FindFlags supportedFindFlags() const;
    void resetIncrementalSearch();
    void clearHighlights();
    QString currentFindString() const;
    QString completedFindString() const;

    Result findIncremental(const QString &txt, FindFlags findFlags);
    Result findStep(const QString &txt, FindFlags findFlags);
    void replace(const QString &before, const QString &after, FindFlags findFlags);
    bool replaceStep(const QString &before, const QString &after, FindFlags findFlags);
    int replaceAll(const QString &before, const QString &after, FindFlags findFlags);

    void defineFindScope();
    void clearFindScope();

    void highlightAll(const QString &txt, FindFlags findFlags);

signals:
    void highlightAllRequested(const QString &txt, Core::FindFlags findFlags);
    void findScopeChanged(const QTextCursor &start, const QTextCursor &end,
                          int verticalBlockSelectionFirstColumn,
                          int verticalBlockSelectionLastColumn);

private:
    bool find(const QString &txt, FindFlags findFlags, QTextCursor start, bool *wrapped);
    QTextCursor replaceInternal(const QString &before, const QString &after, FindFlags findFlags);

    QTextCursor textCursor() const;
    void setTextCursor(const QTextCursor&);
    QTextDocument *document() const;
    bool isReadOnly() const;
    bool inScope(int startPosition, int endPosition) const;
    QTextCursor findOne(const QRegExp &expr, const QTextCursor &from, QTextDocument::FindFlags options) const;

    BaseTextFindPrivate *d;
};

} // namespace Core

#endif // BASETEXTFIND_H
