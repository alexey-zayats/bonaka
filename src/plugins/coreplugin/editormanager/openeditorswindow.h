#ifndef OPENEDITORSWINDOW_H
#define OPENEDITORSWINDOW_H

#include "editorview.h"

#include <QFrame>
#include <QIcon>
#include <QList>
#include <QTreeWidget>

QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
QT_END_NAMESPACE

namespace Core {

class IDocument;
class IEditor;

namespace Internal {

class OpenEditorsTreeWidget : public QTreeWidget {
public:
    explicit OpenEditorsTreeWidget(QWidget *parent = 0) : QTreeWidget(parent) {}
    ~OpenEditorsTreeWidget() {}
    QSize sizeHint() const;
};

class EditorHistoryItem;

class OpenEditorsWindow : public QFrame
{
    Q_OBJECT

public:
    enum Mode {ListMode, HistoryMode };

    explicit OpenEditorsWindow(QWidget *parent = 0);

    void setEditors(const QList<EditLocation> &globalHistory, EditorView *view);

    bool eventFilter(QObject *src, QEvent *e);
    void focusInEvent(QFocusEvent *);
    void setVisible(bool visible);
    void selectNextEditor();
    void selectPreviousEditor();
    QSize sizeHint() const;

public slots:
    void selectAndHide();

private:
    void editorClicked(QTreeWidgetItem *item);
    void selectEditor(QTreeWidgetItem *item);

    void addHistoryItems(const QList<EditLocation> &history, EditorView *view, QSet<IDocument*> &documentsDone);
    void addSuspendedItems();
    void ensureCurrentVisible();
    void selectUpDown(bool up);

    bool isSameFile(IEditor *editorA, IEditor *editorB) const;

    const QIcon m_emptyIcon;
    OpenEditorsTreeWidget *m_editorList;
};

} // namespace Internal
} // namespace Core

#endif // OPENEDITORSWINDOW_H
