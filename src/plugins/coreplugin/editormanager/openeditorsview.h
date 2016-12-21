
#ifndef OPENEDITORSVIEW_H
#define OPENEDITORSVIEW_H

#include <coreplugin/inavigationwidgetfactory.h>
#include <coreplugin/opendocumentstreeview.h>

#include <QAbstractProxyModel>

namespace Core {
class IEditor;

namespace Internal {

class ProxyModel : public QAbstractProxyModel
{
	Q_OBJECT
public:
	explicit ProxyModel(QObject *parent = 0);
	QModelIndex mapFromSource(const QModelIndex & sourceIndex) const;
	QModelIndex mapToSource(const QModelIndex & proxyIndex) const;

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &child) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	void setSourceModel(QAbstractItemModel *sourceModel);

	// QAbstractProxyModel::sibling is broken in Qt 5
	QModelIndex sibling(int row, int column, const QModelIndex &idx) const;
	// QAbstractProxyModel::supportedDragActions delegation is missing in Qt 5
	Qt::DropActions supportedDragActions() const;

private:
	void sourceDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
	void sourceRowsRemoved(const QModelIndex &parent, int start, int end);
	void sourceRowsInserted(const QModelIndex &parent, int start, int end);
	void sourceRowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
	void sourceRowsAboutToBeInserted(const QModelIndex &parent, int start, int end);
};

class OpenEditorsWidget : public OpenDocumentsTreeView
{
	Q_OBJECT

public:
	OpenEditorsWidget();
	~OpenEditorsWidget();

private:
	void handleActivated(const QModelIndex &);
	void updateCurrentItem(IEditor*);
	void contextMenuRequested(QPoint pos);
	void activateEditor(const QModelIndex &index);
	void closeDocument(const QModelIndex &index);

	ProxyModel *m_model;
};

class OpenEditorsViewFactory : public INavigationWidgetFactory
{
	Q_OBJECT

public:
	OpenEditorsViewFactory();

	NavigationView createWidget();
};

} // namespace Internal
} // namespace Core


#endif // OPENEDITORSVIEW_H
