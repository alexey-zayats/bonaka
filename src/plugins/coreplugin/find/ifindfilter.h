#ifndef IFINDFILTER_H
#define IFINDFILTER_H

#include "textfindconstants.h"

QT_BEGIN_NAMESPACE
class QWidget;
class QSettings;
class QKeySequence;
class Pixmap;
QT_END_NAMESPACE

namespace Core {

class CORE_EXPORT IFindFilter : public QObject
{
    Q_OBJECT
public:

    virtual ~IFindFilter() {}

    virtual QString id() const = 0;
    virtual QString displayName() const = 0;
    ///
    virtual bool isEnabled() const = 0;
    virtual bool isValid() const { return true; }
    virtual QKeySequence defaultShortcut() const;
    virtual bool isReplaceSupported() const { return false; }
    virtual FindFlags supportedFindFlags() const;

    virtual void findAll(const QString &txt, FindFlags findFlags) = 0;
    virtual void replaceAll(const QString &txt, FindFlags findFlags)
    { Q_UNUSED(txt) Q_UNUSED(findFlags) }

    virtual QWidget *createConfigWidget() { return 0; }
    virtual void writeSettings(QSettings *settings) { Q_UNUSED(settings) }
    virtual void readSettings(QSettings *settings) { Q_UNUSED(settings) }

    static QPixmap pixmapForFindFlags(FindFlags flags);
    static QString descriptionForFindFlags(FindFlags flags);
signals:
    void enabledChanged(bool enabled);
    void displayNameChanged();
};

} // namespace Core

#endif // IFINDFILTER_H
