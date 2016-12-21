#ifndef COMMANDBUTTON_H
#define COMMANDBUTTON_H

#include <coreplugin/core_global.h>

#include <QPointer>
#include <QString>
#include <QToolButton>

namespace Core {

class Command;
class Id;

class CORE_EXPORT CommandButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(QString toolTipBase READ toolTipBase WRITE setToolTipBase)
public:
    explicit CommandButton(QWidget *parent = 0);
    explicit CommandButton(Id id, QWidget *parent = 0);
    void setCommandId(Id id);
    QString toolTipBase() const;
    void setToolTipBase(const QString &toolTipBase);

private:
    void updateToolTip();

    QPointer<Command> m_command;
    QString m_toolTipBase;
};

}

#endif // COMMANDBUTTON_H
