#ifndef CURRENTDOCUMENTFIND_H
#define CURRENTDOCUMENTFIND_H

#include "ifindsupport.h"

#include <QPointer>

namespace Core {
namespace Internal {

class CurrentDocumentFind : public QObject
{
    Q_OBJECT

public:
    CurrentDocumentFind();

    void resetIncrementalSearch();
    void clearHighlights();
    bool supportsReplace() const;
    FindFlags supportedFindFlags() const;
    QString currentFindString() const;
    QString completedFindString() const;

    bool isEnabled() const;
    IFindSupport *candidate() const;
    void highlightAll(const QString &txt, FindFlags findFlags);
    IFindSupport::Result findIncremental(const QString &txt, FindFlags findFlags);
    IFindSupport::Result findStep(const QString &txt, FindFlags findFlags);
    void replace(const QString &before, const QString &after, FindFlags findFlags);
    bool replaceStep(const QString &before, const QString &after, FindFlags findFlags);
    int replaceAll(const QString &before, const QString &after, FindFlags findFlags);
    void defineFindScope();
    void clearFindScope();
    void acceptCandidate();

    void removeConnections();
    bool setFocusToCurrentFindSupport();

    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void changed();
    void candidateChanged();

private:
    void updateCandidateFindFilter(QWidget *old, QWidget *now);
    void clearFindSupport();
    void aggregationChanged();
    void candidateAggregationChanged();
    void removeFindSupportConnections();

    QPointer<IFindSupport> m_currentFind;
    QPointer<QWidget> m_currentWidget;
    QPointer<IFindSupport> m_candidateFind;
    QPointer<QWidget> m_candidateWidget;
};

} // namespace Internal
} // namespace Core

#endif // CURRENTDOCUMENTFIND_H
