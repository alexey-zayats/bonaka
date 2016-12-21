#ifndef FILEICONPROVIDER_H
#define FILEICONPROVIDER_H

#include <coreplugin/core_global.h>

#include <QStyle>
#include <QFileIconProvider>

namespace Core {

namespace FileIconProvider {

// Access to the single instance
CORE_EXPORT QFileIconProvider *iconProvider();

// Access to individual items
CORE_EXPORT QIcon icon(const QFileInfo &info);
CORE_EXPORT QIcon icon(QFileIconProvider::IconType type);

// Register additional overlay icons
CORE_EXPORT QPixmap overlayIcon(const QPixmap &baseIcon, const QIcon &overlayIcon);
CORE_EXPORT QPixmap overlayIcon(QStyle::StandardPixmap baseIcon, const QIcon &overlayIcon, const QSize &size);
CORE_EXPORT void registerIconOverlayForSuffix(const char *path, const char *suffix);
//CORE_EXPORT void registerIconOverlayForMimeType(const char *path, const char *mimeType);
//CORE_EXPORT void registerIconOverlayForMimeType(const QIcon &icon, const char *mimeType);

} // namespace FileIconProvider
} // namespace Core

#endif // FILEICONPROVIDER_H
