#ifndef IEDITORFACTORY_H
#define IEDITORFACTORY_H

#include <coreplugin/core_global.h>
#include <coreplugin/id.h>

#include <QObject>
#include <QStringList>

namespace Core {

class IEditor;

class CORE_EXPORT IEditorFactory : public QObject
{
    Q_OBJECT

public:
    IEditorFactory(QObject *parent = 0);
    QString displayName() const { return m_displayName; }
    void setDisplayName(const QString &displayName) { m_displayName = displayName; }

    Id id() const { return m_id; }
    void setId(Id id) { m_id = id; }

    virtual IEditor *createEditor() = 0;

    QStringList mimeTypes() const { return m_mimeTypes; }
    void setMimeTypes(const QStringList &mimeTypes) { m_mimeTypes = mimeTypes; }
    void addMimeType(const char *mimeType) { m_mimeTypes.append(QLatin1String(mimeType)); }
    void addMimeType(const QString &mimeType) { m_mimeTypes.append(mimeType); }
private:
    Id m_id;
    QString m_displayName;
    QStringList m_mimeTypes;
};

} // namespace Core

#endif // IEDITORFACTORY_H
