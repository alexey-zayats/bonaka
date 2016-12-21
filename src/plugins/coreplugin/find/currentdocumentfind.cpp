#include "currentdocumentfind.h"

#include <aggregation/aggregate.h>
#include <coreplugin/coreconstants.h>

#include <utils/fadingindicator.h>

#include <QDebug>
#include <QApplication>
#include <QWidget>

using namespace Core;
using namespace Core;
using namespace Core::Internal;

CurrentDocumentFind::CurrentDocumentFind()
  : m_currentFind(0)
{
    connect(qApp, &QApplication::focusChanged,
            this, &CurrentDocumentFind::updateCandidateFindFilter);
}

void CurrentDocumentFind::removeConnections()
{
    disconnect(qApp, 0, this, 0);
    removeFindSupportConnections();
}

void CurrentDocumentFind::resetIncrementalSearch()
{
    m_currentFind->resetIncrementalSearch();
}

void CurrentDocumentFind::clearHighlights()
{
    m_currentFind->clearHighlights();
}

bool CurrentDocumentFind::isEnabled() const
{
    return m_currentFind && (!m_currentWidget || m_currentWidget->isVisible());
}

IFindSupport *CurrentDocumentFind::candidate() const
{
    return m_candidateFind;
}

bool CurrentDocumentFind::supportsReplace() const
{
    return m_currentFind->supportsReplace();
}

FindFlags CurrentDocumentFind::supportedFindFlags() const
{
    return m_currentFind->supportedFindFlags();
}

QString CurrentDocumentFind::currentFindString() const
{
    return m_currentFind->currentFindString();
}

QString CurrentDocumentFind::completedFindString() const
{
    return m_currentFind->completedFindString();
}

void CurrentDocumentFind::highlightAll(const QString &txt, FindFlags findFlags)
{
    m_currentFind->highlightAll(txt, findFlags);
}

IFindSupport::Result CurrentDocumentFind::findIncremental(const QString &txt, FindFlags findFlags)
{
    return m_currentFind->findIncremental(txt, findFlags);
}

IFindSupport::Result CurrentDocumentFind::findStep(const QString &txt, FindFlags findFlags)
{
    return m_currentFind->findStep(txt, findFlags);
}

void CurrentDocumentFind::replace(const QString &before, const QString &after, FindFlags findFlags)
{
    m_currentFind->replace(before, after, findFlags);
}

bool CurrentDocumentFind::replaceStep(const QString &before, const QString &after, FindFlags findFlags)
{
    return m_currentFind->replaceStep(before, after, findFlags);
}

int CurrentDocumentFind::replaceAll(const QString &before, const QString &after, FindFlags findFlags)
{
    int count = m_currentFind->replaceAll(before, after, findFlags);
    Utils::FadingIndicator::showText(m_currentWidget,
                                     tr("%n occurrences replaced.", 0, count),
                                     Utils::FadingIndicator::SmallText);
    return count;
}

void CurrentDocumentFind::defineFindScope()
{
    m_currentFind->defineFindScope();
}

void CurrentDocumentFind::clearFindScope()
{
    m_currentFind->clearFindScope();
}

void CurrentDocumentFind::updateCandidateFindFilter(QWidget *old, QWidget *now)
{
    Q_UNUSED(old)
    QWidget *candidate = now;
    QPointer<IFindSupport> impl = 0;
    while (!impl && candidate) {
        impl = Aggregation::query<IFindSupport>(candidate);
        if (!impl)
            candidate = candidate->parentWidget();
    }
    if (candidate == m_candidateWidget && impl == m_candidateFind)
        return;
    if (m_candidateWidget)
        disconnect(Aggregation::Aggregate::parentAggregate(m_candidateWidget), &Aggregation::Aggregate::changed,
                   this, &CurrentDocumentFind::candidateAggregationChanged);
    m_candidateWidget = candidate;
    m_candidateFind = impl;
    if (m_candidateWidget)
        connect(Aggregation::Aggregate::parentAggregate(m_candidateWidget), &Aggregation::Aggregate::changed,
                this, &CurrentDocumentFind::candidateAggregationChanged);
    emit candidateChanged();
}

void CurrentDocumentFind::acceptCandidate()
{
    if (!m_candidateFind || m_candidateFind == m_currentFind)
        return;
    removeFindSupportConnections();
    if (m_currentFind)
        m_currentFind->clearHighlights();

    if (m_currentWidget)
        disconnect(Aggregation::Aggregate::parentAggregate(m_currentWidget), &Aggregation::Aggregate::changed,
                   this, &CurrentDocumentFind::aggregationChanged);
    m_currentWidget = m_candidateWidget;
    connect(Aggregation::Aggregate::parentAggregate(m_currentWidget), &Aggregation::Aggregate::changed,
            this, &CurrentDocumentFind::aggregationChanged);

    m_currentFind = m_candidateFind;
    if (m_currentFind) {
        connect(m_currentFind.data(), &IFindSupport::changed,
                this, &CurrentDocumentFind::changed);
        connect(m_currentFind.data(), &QObject::destroyed, this, &CurrentDocumentFind::clearFindSupport);
    }
    if (m_currentWidget)
        m_currentWidget->installEventFilter(this);
    emit changed();
}

void CurrentDocumentFind::removeFindSupportConnections()
{
    if (m_currentFind) {
        disconnect(m_currentFind.data(), &IFindSupport::changed,
                   this, &CurrentDocumentFind::changed);
        disconnect(m_currentFind.data(), &IFindSupport::destroyed,
                   this, &CurrentDocumentFind::clearFindSupport);
    }
    if (m_currentWidget)
        m_currentWidget->removeEventFilter(this);
}

void CurrentDocumentFind::clearFindSupport()
{
    removeFindSupportConnections();
    m_currentWidget = 0;
    m_currentFind = 0;
    emit changed();
}

bool CurrentDocumentFind::setFocusToCurrentFindSupport()
{
    if (m_currentFind && m_currentWidget) {
        QWidget *w = m_currentWidget->focusWidget();
        if (!w)
            w = m_currentWidget;
        w->setFocus();
        return true;
    }
    return false;
}

bool CurrentDocumentFind::eventFilter(QObject *obj, QEvent *event)
{
    if (m_currentWidget && obj == m_currentWidget) {
        if (event->type() == QEvent::Hide || event->type() == QEvent::Show)
            emit changed();
    }
    return QObject::eventFilter(obj, event);
}

void CurrentDocumentFind::aggregationChanged()
{
    if (m_currentWidget) {
        QPointer<IFindSupport> currentFind = Aggregation::query<IFindSupport>(m_currentWidget);
        if (currentFind != m_currentFind) {
            // There's a change in the find support
            if (currentFind) {
                m_candidateWidget = m_currentWidget;
                m_candidateFind = currentFind;
                acceptCandidate();
            } else {
                clearFindSupport();
            }
        }
    }
}

void CurrentDocumentFind::candidateAggregationChanged()
{
    if (m_candidateWidget && m_candidateWidget != m_currentWidget) {
        m_candidateFind = Aggregation::query<IFindSupport>(m_candidateWidget);
        emit candidateChanged();
    }
}
