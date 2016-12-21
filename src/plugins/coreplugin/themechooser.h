#ifndef THEMECHOOSER_H
#define THEMECHOOSER_H

#include "id.h"

#include <QWidget>

namespace Core {
namespace Internal {

class ThemeChooserPrivate;

class ThemeEntry
{
public:
    ThemeEntry() = default;
    ThemeEntry(Id id, const QString &filePath);

    Id id() const;
    QString displayName() const;
    QString filePath() const;
    static QList<ThemeEntry> availableThemes();

private:
    Id m_id;
    QString m_filePath;
    mutable QString m_displayName;
};

class ThemeChooser : public QWidget
{
    Q_OBJECT

public:
    ThemeChooser(QWidget *parent = 0);
    ~ThemeChooser();

    void apply();

private:
    ThemeChooserPrivate *d;
};

} // namespace Internal
} // namespace Core

#endif // THEMESETTINGS_H
