#ifndef FILEUTILS_MAC_H
#define FILEUTILS_MAC_H

#include <QUrl>

namespace Utils {
namespace Internal {

QUrl filePathUrl(const QUrl &url);
QString normalizePathName(const QString &filePath);

} // Internal
} // Utils

#endif // FILEUTILS_MAC_H

