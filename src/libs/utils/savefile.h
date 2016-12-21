#ifndef SAVEFILE_H
#define SAVEFILE_H

#include "utils_global.h"

#include <QTemporaryFile>

namespace Utils {

class UTILS_EXPORT SaveFile : public QTemporaryFile
{
public:
    explicit SaveFile(const QString &filename);
    virtual ~SaveFile();

    bool open(OpenMode flags = QIODevice::WriteOnly);

    void rollback();
    bool commit();

    void setBackup(bool backup) { m_backup = backup; }

    static void initializeUmask();

private:
    const QString m_finalFileName;
    bool m_finalized;
    bool m_backup;
    static QFile::Permissions m_umask;
};

} // namespace Utils

#endif // SAVEFILE_H
