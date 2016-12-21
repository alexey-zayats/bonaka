#ifndef TOOLTIPREUSE_H
#define TOOLTIPREUSE_H

#include <utils/hostosinfo.h>

#include <QPoint>
#include <QRect>
#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>

namespace Utils {
namespace Internal {

inline int screenNumber(const QPoint &pos, QWidget *w)
{
	if (QApplication::desktop()->isVirtualDesktop())
		return QApplication::desktop()->screenNumber(pos);
	else
		return QApplication::desktop()->screenNumber(w);
}

inline QRect screenGeometry(const QPoint &pos, QWidget *w)
{
	if (HostOsInfo::isMacHost())
		return QApplication::desktop()->availableGeometry(screenNumber(pos, w));
	return QApplication::desktop()->screenGeometry(screenNumber(pos, w));
}

} // namespace Internal
} // namespace Utils

#endif // TOOLTIPREUSE_H
