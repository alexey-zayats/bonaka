#include "editorwindow.h"

#include "editorarea.h"
#include "editormanager_p.h"

#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>

#include <QVBoxLayout>

namespace Core {
namespace Internal {

EditorWindow::EditorWindow(QWidget *parent) :
    QWidget(parent)
{
    m_area = new EditorArea;
    auto layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);
    layout->addWidget(m_area);
    setFocusProxy(m_area);
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_QuitOnClose, false); // don't prevent Qt Creator from closing
    resize(QSize(800, 600));

    static int windowId = 0;
    ICore::registerWindow(this, Context(Id("EditorManager.ExternalWindow.").withSuffix(++windowId)));

    connect(m_area, &EditorArea::windowTitleNeedsUpdate,
            this, &EditorWindow::updateWindowTitle);
    // editor area can be deleted by editor manager
    connect(m_area, &EditorArea::destroyed, this, [this]() {
        m_area = nullptr;
        deleteLater();
    });
    updateWindowTitle();
}

EditorWindow::~EditorWindow()
{
    if (m_area)
        disconnect(m_area, 0, this, 0);
}

EditorArea *EditorWindow::editorArea() const
{
    return m_area;
}

void EditorWindow::updateWindowTitle()
{
    EditorManagerPrivate::updateWindowTitleForDocument(m_area->currentDocument(), this);
}

} // Internal
} // Core
