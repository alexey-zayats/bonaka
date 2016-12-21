#ifndef IDOCUMENT_H
#define IDOCUMENT_H

#include "core_global.h"

#include <QObject>

namespace Utils { class FileName; }

namespace Core {
class Id;
class InfoBar;

namespace Internal {
class IDocumentPrivate;
}

class CORE_EXPORT IDocument : public QObject
{
    Q_OBJECT

public:
    enum class OpenResult {
        Success,
        ReadError,
        CannotHandle
    };

    // This enum must match the indexes of the reloadBehavior widget
    // in generalsettings.ui
    enum ReloadSetting {
        AlwaysAsk = 0,
        ReloadUnmodified = 1,
        IgnoreAll = 2
    };

    enum ChangeTrigger {
        TriggerInternal,
        TriggerExternal
    };

    enum ChangeType {
        TypeContents,
        TypePermissions,
        TypeRemoved
    };

    enum ReloadBehavior {
        BehaviorAsk,
        BehaviorSilent
    };

    enum ReloadFlag {
        FlagReload,
        FlagIgnore
    };

    IDocument(QObject *parent = 0);
    ~IDocument() override;

    void setId(Id id);
    Id id() const;

    // required to be re-implemented for documents of IEditors
    virtual OpenResult open(QString *errorString, const QString &fileName, const QString &realFileName);

    virtual bool save(QString *errorString, const QString &fileName = QString(), bool autoSave = false);

    virtual QByteArray contents() const;
    virtual bool setContents(const QByteArray &contents);

    const Utils::FileName &filePath() const;
    virtual void setFilePath(const Utils::FileName &filePath);
    QString displayName() const;
    void setPreferredDisplayName(const QString &name);
    QString plainDisplayName() const;
    void setUniqueDisplayName(const QString &name);

    virtual bool isFileReadOnly() const;
    bool isTemporary() const;
    void setTemporary(bool temporary);

    virtual QString fallbackSaveAsPath() const;
    virtual QString fallbackSaveAsFileName() const;

    QString mimeType() const;
    void setMimeType(const QString &mimeType);

    virtual bool shouldAutoSave() const;
    virtual bool isModified() const;
    virtual bool isSaveAsAllowed() const;

    virtual ReloadBehavior reloadBehavior(ChangeTrigger state, ChangeType type) const;
    virtual bool reload(QString *errorString, ReloadFlag flag, ChangeType type);

    virtual void checkPermissions();

    bool autoSave(QString *errorString, const QString &filePath);
    void setRestoredFrom(const QString &name);
    void removeAutoSaveFile();

    bool hasWriteWarning() const;
    void setWriteWarning(bool has);

    InfoBar *infoBar();

signals:
    // For meta data changes: file name, modified state, ...
    void changed();

    // For changes in the contents of the document
    void contentsChanged();

    void mimeTypeChanged();

    void aboutToReload();
    void reloadFinished(bool success);

    void filePathChanged(const Utils::FileName &oldName, const Utils::FileName &newName);

private:
    Internal::IDocumentPrivate *d;
};

} // namespace Core

#endif // IDOCUMENT_H
