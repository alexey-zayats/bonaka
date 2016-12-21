#ifndef SHORTCUTSETTINGS_H
#define SHORTCUTSETTINGS_H

#include <coreplugin/actionmanager/commandmappings.h>
#include <coreplugin/dialogs/ioptionspage.h>

#include <QKeySequence>
#include <QPointer>
#include <QPushButton>

#include <array>

QT_BEGIN_NAMESPACE
class QGroupBox;
class QKeyEvent;
class QLabel;
QT_END_NAMESPACE

namespace Core {

class Command;

namespace Internal {

class ActionManagerPrivate;
class MainWindow;

struct ShortcutItem
{
    Command *m_cmd;
    QKeySequence m_key;
    QTreeWidgetItem *m_item;
};

class ShortcutButton : public QPushButton
{
    Q_OBJECT
public:
    ShortcutButton(QWidget *parent = 0);

    QSize sizeHint() const;

signals:
    void keySequenceChanged(const QKeySequence &sequence);

protected:
    bool eventFilter(QObject *obj, QEvent *evt);

private:
    void updateText();
    void handleToggleChange(bool toggleState);

    QString m_checkedText;
    QString m_uncheckedText;
    mutable int m_preferredWidth = -1;
    std::array<int, 4> m_key;
    int m_keyNum = 0;
};

class ShortcutSettingsWidget : public CommandMappings
{
    Q_OBJECT

public:
    ShortcutSettingsWidget(QWidget *parent = 0);
    ~ShortcutSettingsWidget() override;

    void apply();

protected:
    void importAction() override;
    void exportAction() override;
    void defaultAction() override;
    bool filterColumn(const QString &filterString, QTreeWidgetItem *item, int column) const override;

private:
    void initialize();
    void handleCurrentCommandChanged(QTreeWidgetItem *current);
    void resetToDefault();
    bool validateShortcutEdit() const;
    bool markCollisions(ShortcutItem *);
    void setKeySequence(const QKeySequence &key);
    void showConflicts();
    void clear();

    QList<ShortcutItem *> m_scitems;
    QGroupBox *m_shortcutBox;
    Utils::LineEdit *m_shortcutEdit;
    QLabel *m_warningLabel;
};

class ShortcutSettings : public IOptionsPage
{
    Q_OBJECT

public:
    ShortcutSettings(QObject *parent = 0);

    QWidget *widget() override;
    void apply() override;
    void finish() override;

private:
    QPointer<ShortcutSettingsWidget> m_widget;
};

} // namespace Internal
} // namespace Core

#endif // SHORTCUTSETTINGS_H
