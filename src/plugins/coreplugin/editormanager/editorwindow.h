#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QWidget>

namespace Core {
namespace Internal {

class EditorArea;

class EditorWindow : public QWidget
{
    Q_OBJECT
public:
    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();

    EditorArea *editorArea() const;

private:
    void updateWindowTitle();

    EditorArea *m_area;
};

} // Internal
} // Core

#endif // EDITORWINDOW_H
