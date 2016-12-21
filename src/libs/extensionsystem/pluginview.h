#ifndef PLUGINVIEW_H
#define PLUGINVIEW_H

#include "extensionsystem_global.h"

#include <QWidget>
#include <QSet>
#include <QHash>

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
QT_END_NAMESPACE

namespace Utils {
class TreeItem;
class TreeModel;
class TreeView;
} // namespace Utils

namespace ExtensionSystem {

class PluginManager;
class PluginSpec;

namespace Internal {
class PluginItem;
class CollectionItem;
} // Internal

class EXTENSIONSYSTEM_EXPORT PluginView : public QWidget
{
    Q_OBJECT

public:
    explicit PluginView(QWidget *parent = 0);
    ~PluginView();

    PluginSpec *currentPlugin() const;
    void setFilter(const QString &filter);

signals:
    void currentPluginChanged(ExtensionSystem::PluginSpec *spec);
    void pluginActivated(ExtensionSystem::PluginSpec *spec);
    void pluginSettingsChanged(ExtensionSystem::PluginSpec *spec);

private:
    PluginSpec *pluginForIndex(const QModelIndex &index) const;
    void updatePlugins();
    bool setPluginsEnabled(const QSet<PluginSpec *> &plugins, bool enable);

    Utils::TreeView *m_categoryView;
    Utils::TreeModel *m_model;
    QSortFilterProxyModel *m_sortModel;

    friend class Internal::CollectionItem;
    friend class Internal::PluginItem;
    friend class PluginSpec;
};

} // namespae ExtensionSystem

#endif // PLUGIN_VIEW_H
