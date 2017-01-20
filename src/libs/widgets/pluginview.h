#ifndef PLUGINVIEW_H
#define PLUGINVIEW_H

#include "widgets_global.h"
#include <utils/treemodel.h>

#include <QWidget>
#include <QSet>
#include <QHash>

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
QT_END_NAMESPACE

namespace Utils {
class TreeItem;
class TreeView;
} // namespace Utils

namespace ExtensionSystem {
class PluginManager;
class PluginSpec;
}

namespace Widgets {

namespace Internal {
class PluginItem;
class CollectionItem;
} // Internal

class WIDGETS_EXPORT PluginView : public QWidget
{
    Q_OBJECT

public:
    explicit PluginView(QWidget *parent = 0);
    ~PluginView();

    ExtensionSystem::PluginSpec *currentPlugin() const;
    void setFilter(const QString &filter);

signals:
    void currentPluginChanged(ExtensionSystem::PluginSpec *spec);
    void pluginActivated(ExtensionSystem::PluginSpec *spec);
    void pluginSettingsChanged(ExtensionSystem::PluginSpec *spec);

private:
    ExtensionSystem::PluginSpec *pluginForIndex(const QModelIndex &index) const;
    void updatePlugins();
    bool setPluginsEnabled(const QSet<ExtensionSystem::PluginSpec *> &plugins, bool enable);

    Utils::TreeView *m_categoryView;
    Utils::TreeModel<Utils::TreeItem, Internal::CollectionItem, Internal::PluginItem> *m_model;
    QSortFilterProxyModel *m_sortModel;

    friend class Internal::CollectionItem;
    friend class Internal::PluginItem;
    friend class ExtensionSystem::PluginSpec;
};

} // namespae Widgets

#endif // PLUGIN_VIEW_H
