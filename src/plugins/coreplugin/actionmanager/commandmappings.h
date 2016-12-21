#ifndef COMMANDMAPPINGS_H
#define COMMANDMAPPINGS_H

#include <coreplugin/core_global.h>

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QTreeWidget;
class QTreeWidgetItem;
QT_END_NAMESPACE

namespace Utils { class LineEdit; }

namespace Core {

namespace Internal { class CommandMappingsPrivate; }

class CORE_EXPORT CommandMappings : public QWidget
{
    Q_OBJECT

public:
    CommandMappings(QWidget *parent = 0);
    ~CommandMappings();

signals:
    void currentCommandChanged(QTreeWidgetItem *current);

protected:
    virtual void defaultAction() = 0;

    virtual void exportAction() {}
    virtual void importAction() {}

    virtual bool filterColumn(const QString &filterString, QTreeWidgetItem *item, int column) const;

    void filterChanged(const QString &f);

    // access to m_page
    void setImportExportEnabled(bool enabled);
    QTreeWidget *commandList() const;
    QString filterText() const;
    void setFilterText(const QString &text);
    void setPageTitle(const QString &s);
    void setTargetHeader(const QString &s);
    void setModified(QTreeWidgetItem *item, bool modified);

private:
    bool filter(const QString &filterString, QTreeWidgetItem *item);

    friend class Internal::CommandMappingsPrivate;
    Internal::CommandMappingsPrivate *d;
};

} // namespace Core

#endif // COMMANDMAPPINGS_H
