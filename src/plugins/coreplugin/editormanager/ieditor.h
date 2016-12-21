#ifndef IEDITOR_H
#define IEDITOR_H

#include <coreplugin/core_global.h>
#include <coreplugin/icontext.h>

#include <QMetaType>

namespace Core {

class IDocument;

class CORE_EXPORT IEditor : public IContext
{
    Q_OBJECT

public:
    IEditor(QObject *parent = 0);

    bool duplicateSupported() const;
    void setDuplicateSupported(bool duplicateSupported);

    virtual IDocument *document() = 0;

    virtual IEditor *duplicate() { return 0; }

    virtual QByteArray saveState() const { return QByteArray(); }
    virtual bool restoreState(const QByteArray &/*state*/) { return true; }

    virtual int currentLine() const { return 0; }
    virtual int currentColumn() const { return 0; }
    virtual void gotoLine(int line, int column = 0, bool centerLine = true) { Q_UNUSED(line) Q_UNUSED(column) Q_UNUSED(centerLine) }

    virtual QWidget *toolBar() = 0;

    virtual bool isDesignModePreferred() const { return false; }

private:
    bool m_duplicateSupported;
};

} // namespace Core

Q_DECLARE_METATYPE(Core::IEditor*)

#endif // IEDITOR_H
