
#ifndef ICONTEXT_H
#define ICONTEXT_H

#include <coreplugin/core_global.h>
#include <coreplugin/id.h>

#include <QList>
#include <QObject>
#include <QPointer>
#include <QWidget>

namespace Core {

class CORE_EXPORT Context
{
public:
    Context() {}

    explicit Context(Id c1) { add(c1); }
    Context(Id c1, Id c2) { add(c1); add(c2); }
    Context(Id c1, Id c2, Id c3) { add(c1); add(c2); add(c3); }
    bool contains(Id c) const { return d.contains(c); }
    int size() const { return d.size(); }
    bool isEmpty() const { return d.isEmpty(); }
    Id at(int i) const { return d.at(i); }

    // FIXME: Make interface slimmer.
    typedef QList<Id>::const_iterator const_iterator;
    const_iterator begin() const { return d.begin(); }
    const_iterator end() const { return d.end(); }
    int indexOf(Id c) const { return d.indexOf(c); }
    void removeAt(int i) { d.removeAt(i); }
    void prepend(Id c) { d.prepend(c); }
    void add(const Context &c) { d += c.d; }
    void add(Id c) { d.append(c); }
    bool operator==(const Context &c) const { return d == c.d; }

private:
    QList<Id> d;
};

class CORE_EXPORT IContext : public QObject
{
    Q_OBJECT
public:
    IContext(QObject *parent = 0) : QObject(parent) {}

    virtual Context context() const { return m_context; }
    virtual QWidget *widget() const { return m_widget; }
    virtual QString contextHelpId() const { return m_contextHelpId; }

    virtual void setContext(const Context &context) { m_context = context; }
    virtual void setWidget(QWidget *widget) { m_widget = widget; }
    virtual void setContextHelpId(const QString &id) { m_contextHelpId = id; }

protected:
    Context m_context;
    QPointer<QWidget> m_widget;
    QString m_contextHelpId;
};

} // namespace Core

#endif //ICONTEXT_H
